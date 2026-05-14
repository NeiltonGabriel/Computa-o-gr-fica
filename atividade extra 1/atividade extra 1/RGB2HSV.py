import tkinter as tk
from tkinter import filedialog
import numpy as np
from PIL import Image
import sys

try:
    from OpenGL.GL import *
    from OpenGL.GLU import *
except ImportError:
    print("Erro: PyOpenGL não encontrado. Instale com: pip install PyOpenGL PyOpenGL_accelerate")
    sys.exit(1)

try:
    from pyopengltk import OpenGLFrame
    USE_PYOPENGLTK = True
except ImportError:
    USE_PYOPENGLTK = False

def rgb_to_hsv_array(img_rgb: np.ndarray) -> np.ndarray:
    r, g, b = img_rgb[..., 0], img_rgb[..., 1], img_rgb[..., 2]
    maxc = np.max(img_rgb, axis=2)
    minc = np.min(img_rgb, axis=2)
    v = maxc
    s = np.where(maxc != 0, (maxc - minc) / maxc, 0.0)
    diff = maxc - minc
    diff_safe = np.where(diff == 0, 1, diff)

    h = np.zeros_like(maxc)
    mask_r = (maxc == r) & (diff != 0)
    mask_g = (maxc == g) & (diff != 0)
    mask_b = (maxc == b) & (diff != 0)

    h[mask_r] = ((g[mask_r] - b[mask_r]) / diff_safe[mask_r]) % 6
    h[mask_g] = (b[mask_g] - r[mask_g]) / diff_safe[mask_g] + 2
    h[mask_b] = (r[mask_b] - g[mask_b]) / diff_safe[mask_b] + 4
    h = h / 6.0

    return np.stack([h, s, v], axis=2).astype(np.float32)


def hsv_to_rgb_array(hsv: np.ndarray) -> np.ndarray:
    h = hsv[..., 0] * 6.0
    s = hsv[..., 1]
    v = hsv[..., 2]

    i = np.floor(h).astype(int) % 6
    f = h - np.floor(h)
    p = v * (1 - s)
    q = v * (1 - f * s)
    t = v * (1 - (1 - f) * s)

    rgb = np.zeros((*h.shape, 3), dtype=np.float32)
    for idx, (r_, g_, b_) in enumerate([
        (v, t, p), (q, v, p), (p, v, t),
        (p, q, v), (t, p, v), (v, p, q)
    ]):
        mask = i == idx
        rgb[mask, 0] = r_[mask]
        rgb[mask, 1] = g_[mask]
        rgb[mask, 2] = b_[mask]

    return np.clip(rgb, 0, 1)


def apply_hsv_shift(hsv_base, dh, ds, dv):
    hsv = hsv_base.copy()
    hsv[..., 0] = (hsv[..., 0] + dh) % 1.0
    hsv[..., 1] = np.clip(hsv[..., 1] + ds, 0, 1)
    hsv[..., 2] = np.clip(hsv[..., 2] + dv, 0, 1)
    return hsv

if USE_PYOPENGLTK:
    class GLCanvas(OpenGLFrame):
        def __init__(self, master, **kw):
            super().__init__(master, **kw)
            self.texture_id = None
            self.img_data   = None
            self.tex_size   = (1, 1)
            self._gl_ready  = False

        def initgl(self):
            glEnable(GL_TEXTURE_2D)
            self.texture_id = glGenTextures(1)
            self._gl_ready  = True

        def redraw(self):
            glClearColor(0.07, 0.07, 0.10, 1.0)
            glClear(GL_COLOR_BUFFER_BIT)
            if self.img_data is None or not self._gl_ready:
                return

            win_w = max(self.winfo_width(),  1)
            win_h = max(self.winfo_height(), 1)
            tex_w, tex_h = self.tex_size

            scale  = min(win_w / tex_w, win_h / tex_h)
            draw_w = tex_w * scale
            draw_h = tex_h * scale
            x0 = (win_w - draw_w) / 2
            y0 = (win_h - draw_h) / 2
            x1 = x0 + draw_w
            y1 = y0 + draw_h

            glMatrixMode(GL_PROJECTION)
            glLoadIdentity()
            glOrtho(0, win_w, 0, win_h, -1, 1)
            glMatrixMode(GL_MODELVIEW)
            glLoadIdentity()

            glBindTexture(GL_TEXTURE_2D, self.texture_id)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         tex_w, tex_h, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, self.img_data)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

            glColor3f(1, 1, 1)
            glBegin(GL_QUADS)
            glTexCoord2f(0, 1); glVertex2f(x0, y0)
            glTexCoord2f(1, 1); glVertex2f(x1, y0)
            glTexCoord2f(1, 0); glVertex2f(x1, y1)
            glTexCoord2f(0, 0); glVertex2f(x0, y1)
            glEnd()

        def update_texture(self, rgb_array: np.ndarray):
            rgb8  = (rgb_array * 255).astype(np.uint8)
            h, w  = rgb8.shape[:2]
            rgba8 = np.ones((h, w, 4), dtype=np.uint8) * 255
            rgba8[..., :3] = rgb8
            self.tex_size  = (w, h)
            self.img_data  = rgba8.tobytes()
            self.tkExpose(None)

else:
    from PIL import ImageTk

    class GLCanvas(tk.Canvas):
        def __init__(self, master, **kw):
            kw.setdefault('bg', '#121218')
            super().__init__(master, **kw)
            self._photo = None

        def initgl(self): pass

        def update_texture(self, rgb_array: np.ndarray):
            rgb8 = (np.clip(rgb_array, 0, 1) * 255).astype(np.uint8)
            pil  = Image.fromarray(rgb8, 'RGB')
            cw   = self.winfo_width()  or 680
            ch   = self.winfo_height() or 420
            pil.thumbnail((cw, ch), Image.LANCZOS)
            self._photo = ImageTk.PhotoImage(pil)
            self.delete("all")
            self.create_image(cw // 2, ch // 2, anchor='center', image=self._photo)


class HSVSlider(tk.Canvas):
    HEIGHT  = 38
    TRACK_H = 6
    THUMB_R = 10

    def __init__(self, master, from_: float, to: float, init: float,
                 color: str, command=None, **kw):
        kw['height']            = self.HEIGHT
        kw['bg']                = '#22222e'
        kw['highlightthickness'] = 0
        kw['bd']                = 0
        super().__init__(master, **kw)

        self._from  = from_
        self._to    = to
        self._value = float(init)
        self._color = color
        self._cmd   = command
        self._drag  = False

        self.bind("<Configure>",       self._redraw)
        self.bind("<ButtonPress-1>",   self._on_press)
        self.bind("<B1-Motion>",       self._on_drag)
        self.bind("<ButtonRelease-1>", self._on_release)


    def _redraw(self, _=None):
        self.delete("all")
        w  = max(self.winfo_width(), 1)
        h  = self.HEIGHT
        pad = self.THUMB_R + 2
        cw  = w - 2 * pad
        cy  = h // 2

        frac = (self._value - self._from) / max(self._to - self._from, 1e-9)
        frac = max(0.0, min(1.0, frac))

        self._rrect(pad, cy - self.TRACK_H // 2,
                    pad + cw, cy + self.TRACK_H // 2,
                    r=3, fill="#3a3a4e", outline="")

        fx = pad + frac * cw
        if fx > pad + 1:
            self._rrect(pad, cy - self.TRACK_H // 2,
                        fx, cy + self.TRACK_H // 2,
                        r=3, fill=self._color, outline="")

        self.create_oval(fx - self.THUMB_R, cy - self.THUMB_R,
                         fx + self.THUMB_R, cy + self.THUMB_R,
                         fill=self._color, outline="#1a1a22", width=2)

    def _rrect(self, x1, y1, x2, y2, r=4, **kw):
        pts = [x1+r, y1,  x2-r, y1,  x2, y1,   x2, y1+r,
               x2, y2-r, x2, y2,    x2-r, y2,  x1+r, y2,
               x1, y2,   x1, y2-r,  x1, y1+r,  x1, y1]
        self.create_polygon(pts, smooth=True, **kw)


    def _px_to_value(self, x):
        w   = max(self.winfo_width(), 1)
        pad = self.THUMB_R + 2
        cw  = w - 2 * pad
        frac = (x - pad) / max(cw, 1)
        return self._from + max(0.0, min(1.0, frac)) * (self._to - self._from)

    def _on_press(self, e):
        self._drag  = True
        self._value = self._px_to_value(e.x)
        self._redraw()
        if self._cmd:
            self._cmd(self._value)

    def _on_drag(self, e):
        if self._drag:
            self._value = self._px_to_value(e.x)
            self._redraw()
            if self._cmd:
                self._cmd(self._value)

    def _on_release(self, e):
        self._drag = False


    def get(self) -> float:
        return self._value

    def set(self, value: float):
        self._value = max(self._from, min(self._to, float(value)))
        self._redraw()


class App(tk.Tk):

    def __init__(self):
        super().__init__()
        self.title("RGB → HSV Editor  |  OpenGL")
        self.configure(bg="#1a1a22")
        self.resizable(True, True)
        self.hsv_base = None
        self._build_ui()
        self.minsize(760, 560)


    def _build_ui(self):
        authors_bar = tk.Frame(self, bg="#12121a", pady=5)
        authors_bar.pack(fill="x")

        tk.Label(authors_bar,
                 text="✦  Jadiel Lino   ·   Neilton Luciano  ✦",
                 font=("Courier New", 10, "bold"),
                 fg="#5a6a8a", bg="#12121a").pack()

        tk.Frame(self, bg="#00ffc8", height=1).pack(fill="x")


        header = tk.Frame(self, bg="#1a1a22", pady=10)
        header.pack(fill="x", padx=20)

        tk.Label(header, text="RGB → HSV",
                 font=("Courier New", 22, "bold"),
                 fg="#00ffc8", bg="#1a1a22").pack(side="left")
        tk.Label(header, text="editor em tempo real",
                 font=("Courier New", 10), fg="#557a6e",
                 bg="#1a1a22").pack(side="left", padx=12)

        tk.Button(header, text="＋  Carregar Imagem",
                  font=("Courier New", 11, "bold"),
                  bg="#00ffc8", fg="#0a0a10", relief="flat",
                  padx=14, pady=6, cursor="hand2",
                  command=self._load_image).pack(side="right")

        cf = tk.Frame(self, bg="#0d0d14", padx=4, pady=4)
        cf.pack(fill="both", expand=True, padx=20, pady=(0, 8))

        self.gl_canvas = GLCanvas(cf, width=680, height=420)
        self.gl_canvas.pack(fill="both", expand=True)

        self._ph = tk.Label(
            self.gl_canvas,
            text="Nenhuma imagem carregada\nClique em  ＋ Carregar Imagem",
            font=("Courier New", 13), fg="#3a4a42",
            bg="#0d0d14", justify="center"
        )
        self._ph.place(relx=0.5, rely=0.5, anchor="center")

        # Painel de sliders
        ctrl = tk.Frame(self, bg="#1a1a22", pady=4)
        ctrl.pack(fill="x", padx=20, pady=(0, 14))
        ctrl.columnconfigure(0, weight=1)
        ctrl.columnconfigure(1, weight=1)
        ctrl.columnconfigure(2, weight=1)

        self._sl_h = self._make_slider(
            ctrl, col=0,
            label="Matiz (H)", color="#ff6b6b",
            from_=0,    to=360, init=0,
            fmt=lambda v: f"{v:.0f}°",
            on_change=lambda _: self._render()
        )
        self._sl_s = self._make_slider(
            ctrl, col=1,
            label="Saturação (S)", color="#ffd93d",
            from_=-100, to=100, init=0,
            fmt=lambda v: f"{v:+.0f}%",
            on_change=lambda _: self._render()
        )
        self._sl_v = self._make_slider(
            ctrl, col=2,
            label="Brilho (V)", color="#6bcb77",
            from_=-100, to=100, init=0,
            fmt=lambda v: f"{v:+.0f}%",
            on_change=lambda _: self._render()
        )

        tk.Button(ctrl, text="↺  Reset",
                  font=("Courier New", 10, "bold"),
                  bg="#2a2a36", fg="#aaaacc", relief="flat",
                  padx=14, pady=4, cursor="hand2",
                  command=self._reset).grid(row=2, column=0, columnspan=3,
                                            pady=(8, 0))

    def _make_slider(self, parent, col, label, color,
                     from_, to, init, fmt, on_change):
        cell = tk.Frame(parent, bg="#22222e", padx=14, pady=10)
        cell.grid(row=0, column=col, padx=8, sticky="ew")

        tk.Label(cell, text=label,
                 font=("Courier New", 10, "bold"),
                 fg=color, bg="#22222e").pack(anchor="w")

        var = tk.StringVar(value=fmt(init))
        tk.Label(cell, textvariable=var,
                 font=("Courier New", 15, "bold"),
                 fg="#e8e8ff", bg="#22222e").pack(anchor="w", pady=(2, 6))

        def _cb(val):
            var.set(fmt(val))
            on_change(val)

        sl = HSVSlider(cell, from_=from_, to=to, init=init,
                       color=color, command=_cb)
        sl.pack(fill="x")

        sl._var = var
        sl._fmt = fmt
        return sl


    def _reset(self):
        for sl in (self._sl_h, self._sl_s, self._sl_v):
            sl.set(0)
            sl._var.set(sl._fmt(0))
        self._render()

    def _load_image(self):
        path = filedialog.askopenfilename(
            title="Selecionar imagem",
            filetypes=[("Imagens", "*.png *.jpg *.jpeg *.bmp *.tiff *.webp"),
                       ("Todos os arquivos", "*.*")]
        )
        if not path:
            return
        try:
            pil = Image.open(path).convert("RGB")
            pil.thumbnail((1024, 1024), Image.LANCZOS)
            arr = np.array(pil, dtype=np.float32) / 255.0
            self.hsv_base = rgb_to_hsv_array(arr)
            self._ph.place_forget()
            self._render()
        except Exception as e:
            from tkinter import messagebox
            messagebox.showerror("Erro ao carregar imagem", str(e))

    def _render(self):
        if self.hsv_base is None:
            return
        dh =  self._sl_h.get() / 360.0
        ds =  self._sl_s.get() / 100.0
        dv =  self._sl_v.get() / 100.0
        hsv_adj = apply_hsv_shift(self.hsv_base, dh, ds, dv)
        rgb_out = hsv_to_rgb_array(hsv_adj)
        self.gl_canvas.update_texture(rgb_out)


if __name__ == "__main__":
    if not USE_PYOPENGLTK:
        print("[AVISO] pyopengltk não encontrado. Usando canvas Tkinter.")
        print("        Para OpenGL nativo: pip install pyopengltk")
    App().mainloop()
