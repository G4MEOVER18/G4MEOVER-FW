#!/usr/bin/env python3
"""Generiert originale 1-bit-Idle-Animationen (128x64) fuer den G4MEOVER-Pack:
der Skull jagt / bekaempft / vertreibt den Flipper-Delfin. Schreibt frame_N.png +
meta.txt je Anim und aktualisiert das Pack-manifest.txt.
Konvention wie bestehende Flipper-Anim-PNGs: weisser Hintergrund (1), schwarze Kunst (0)."""
import os, math
from PIL import Image, ImageDraw

PACK = r"C:\Data\KI\github\G4MEOVER-FW-v2\assets\packs\G4MEOVER\Anims"
W, H = 128, 64
BLACK, WHITE = 0, 1

def new_frame():
    img = Image.new("1", (W, H), WHITE)
    return img, ImageDraw.Draw(img)

# ---------- Delfin ----------
def draw_dolphin(d, cx, cy, s=1.0, face=1, state="ok"):
    """face: +1 rechts, -1 links. state: ok|scared|dead|fade"""
    def X(x):  # spiegeln je nach Blickrichtung
        return cx + face * x * s
    def Y(y):
        return cy + y * s
    def ell(a, b, c, e):  # Ellipse mit normalisierter Bounding-Box (face=-1-sicher)
        d.ellipse([min(a, c), min(b, e), max(a, c), max(b, e)], fill=BLACK)
    # Koerper
    ell(X(-22), Y(-9), X(14), Y(9))
    # Schnauze
    d.polygon([(X(14), Y(-4)), (X(24), Y(-1)), (X(14), Y(4))], fill=BLACK)
    # Schwanzflosse
    d.polygon([(X(-22), Y(0)), (X(-34), Y(-8)), (X(-28), Y(0)), (X(-34), Y(8))], fill=BLACK)
    # Rueckenflosse
    d.polygon([(X(-4), Y(-9)), (X(2), Y(-20)), (X(6), Y(-9))], fill=BLACK)
    # Brustflosse
    d.polygon([(X(-2), Y(6)), (X(-8), Y(15)), (X(4), Y(8))], fill=BLACK)
    # Auge (weisses Loch)
    ex, ey = X(7), Y(-3)
    if state == "dead":
        # X-Augen
        d.line([(ex - 3, ey - 3), (ex + 3, ey + 3)], fill=WHITE, width=1)
        d.line([(ex - 3, ey + 3), (ex + 3, ey - 3)], fill=WHITE, width=1)
    else:
        d.ellipse([ex - 2, ey - 2, ex + 2, ey + 2], fill=WHITE)
        d.ellipse([ex - 1, ey - 1, ex + 1, ey + 1], fill=BLACK)
    if state == "scared":
        # Schweisstropfen ueber dem Kopf
        ell(X(10), Y(-16), X(14), Y(-11))

def dolphin_fade(d, cx, cy, level, face=-1):
    """Aufloesungs-/Glitch-Effekt: nur Reststreifen des Delfins zeichnen."""
    full, fd = new_frame()
    draw_dolphin(fd, cx, cy, 1.0, face, "scared")
    px = full.load()
    keep = max(0, 6 - level)  # je hoeher level, desto weniger bleibt
    for y in range(H):
        # abwechselnde Zeilen loeschen + horizontal versetzt (Glitch)
        if (y % 6) >= keep:
            continue
        shift = ((y * 7 + level * 5) % 11) - 5
        for x in range(W):
            if px[x, y] == BLACK:
                nx = x + shift
                if 0 <= nx < W:
                    d.point((nx, y), fill=BLACK)
    # ein paar Glitch-Splitter
    for i in range(level):
        gy = (cy - 8 + i * 4) % H
        gx = cx + ((i * 13) % 20) - 10
        d.rectangle([gx, gy, gx + 3, gy + 1], fill=BLACK)

# ---------- Skull ----------
def draw_skull(d, cx, cy, s=1.0, rage=0):
    """rage 0..2 groesser/aggressiver."""
    r = 15 * s + rage
    # Schaedel (schwarze Silhouette)
    d.ellipse([cx - r, cy - r, cx + r, cy + r * 0.9], fill=BLACK)
    # Kiefer
    d.rectangle([cx - r * 0.6, cy + r * 0.5, cx + r * 0.6, cy + r * 1.15], fill=BLACK)
    # Augenhoehlen (weiss)
    eo = r * 0.42
    ew = r * 0.34
    for sx in (-1, 1):
        exc = cx + sx * eo
        eyc = cy - r * 0.12
        d.ellipse([exc - ew, eyc - ew, exc + ew, eyc + ew], fill=WHITE)
        # gluehende Pupille (schwarzer Punkt) - bei rage zusammengekniffen
        pd = 2 if rage else 1
        d.ellipse([exc - pd, eyc - pd, exc + pd, eyc + pd], fill=BLACK)
    # Nase (weisses Dreieck)
    d.polygon([(cx, cy + r * 0.15), (cx - r * 0.14, cy + r * 0.45),
               (cx + r * 0.14, cy + r * 0.45)], fill=WHITE)
    # Zaehne (weisse Spalten im Kiefer)
    jt = cy + r * 0.55
    jb = cy + r * 1.1
    for tx in range(-2, 3):
        xx = cx + tx * (r * 0.22)
        d.line([(xx, jt), (xx, jb)], fill=WHITE, width=1)
    d.line([(cx - r * 0.55, jt), (cx + r * 0.55, jt)], fill=WHITE, width=1)

def motion_lines(d, x, y, n=3, length=10, gap=5, face=1):
    for i in range(n):
        yy = y + (i - n // 2) * gap
        d.line([(x, yy), (x - face * length, yy)], fill=BLACK, width=1)

def lightning(d, x0, y0, x1, y1):
    midx = (x0 + x1) // 2
    d.line([(x0, y0), (midx, y0 - 6), (midx + 4, y0 + 4), (x1, y1)], fill=BLACK, width=2)

# ---------- Animationen ----------
def anim_hunt():
    """Skull jagt fliehenden Delfin nach rechts; Delfin schwitzt, Skull rueckt nach."""
    frames = []
    for i in range(8):
        img, d = new_frame()
        prog = i / 7.0
        dolph_x = 40 + int(prog * 60)
        skull_x = 2 + int(prog * 60)
        bob = int(3 * math.sin(i * 1.1))
        draw_dolphin(d, dolph_x, 30 + bob, 1.0, face=1, state="scared")
        motion_lines(d, dolph_x - 30, 30 + bob, n=3, length=8 + i, face=1)
        draw_skull(d, skull_x, 34, 1.0, rage=1 if i % 2 else 0)
        frames.append(img)
    return frames

def anim_zap():
    """Skull mittig, Blitz trifft Delfin -> Delfin kippt mit X-Augen."""
    frames = []
    for i in range(8):
        img, d = new_frame()
        draw_skull(d, 26, 34, 1.0, rage=2 if i >= 3 else 1)
        if i < 3:  # aufladen: Delfin noch ok, naehert sich
            draw_dolphin(d, 92 - i * 2, 30, 1.0, face=-1, state="scared")
        elif i < 5:  # Blitz
            lightning(d, 44, 30, 84, 30)
            draw_dolphin(d, 92, 30, 1.0, face=-1, state="scared")
        else:  # KO: Delfin auf dem Ruecken, X-Augen, Sterne
            img2, d2 = new_frame()
            draw_skull(d2, 26, 34, 1.0, rage=2)
            # Delfin gekippt: einfach vertikal gespiegelt zeichnen
            tmp, td = new_frame()
            draw_dolphin(td, 92, 30, 1.0, face=-1, state="dead")
            tmp = tmp.transpose(Image.FLIP_TOP_BOTTOM)
            img2.paste(tmp, (0, 0), None)
            d2 = ImageDraw.Draw(img2)
            for sx, sy in [(84, 14), (100, 12), (92, 8)]:
                d2.line([(sx - 2, sy), (sx + 2, sy)], fill=BLACK)
                d2.line([(sx, sy - 2), (sx, sy + 2)], fill=BLACK)
            img = img2
        frames.append(img)
    return frames

def anim_vanish():
    """Skull looms links, Delfin rechts loest sich in Glitch auf."""
    frames = []
    for i in range(8):
        img, d = new_frame()
        rage = 2 if i >= 4 else 1
        draw_skull(d, 22 + i, 34, 1.0 + i * 0.03, rage=rage)
        if i < 2:
            draw_dolphin(d, 92, 30, 1.0, face=-1, state="scared")
        else:
            dolphin_fade(d, 92, 30, level=i - 1, face=-1)
        frames.append(img)
    return frames

ANIMS = {
    "G4MEOVER_Hunt_128x64":  (anim_hunt,   {"weight": 7, "minlvl": 1, "maxlvl": 30}),
    "G4MEOVER_Zap_128x64":   (anim_zap,    {"weight": 6, "minlvl": 1, "maxlvl": 30}),
    "G4MEOVER_Vanish_128x64":(anim_vanish, {"weight": 6, "minlvl": 1, "maxlvl": 30}),
}

def write_meta(folder, nframes, rate=4):
    order = " ".join(str(i) for i in range(nframes))
    meta = f"""Filetype: Flipper Animation
Version: 1

Width: 128
Height: 64
Passive frames: {nframes}
Active frames: 0
Frames order: {order}
Active cycles: 0
Frame rate: {rate}
Duration: 3600
Active cooldown: 0

Bubble slots: 0
"""
    with open(os.path.join(folder, "meta.txt"), "w", newline="\n") as f:
        f.write(meta)

created = []
for name, (fn, opts) in ANIMS.items():
    folder = os.path.join(PACK, name)
    os.makedirs(folder, exist_ok=True)
    frames = fn()
    for idx, im in enumerate(frames):
        im.save(os.path.join(folder, f"frame_{idx}.png"))
    write_meta(folder, len(frames))
    created.append((name, len(frames), opts))
    print(f"  {name}: {len(frames)} Frames")

# manifest.txt neu schreiben (bestehende Attract + neue)
man = os.path.join(PACK, "manifest.txt")
existing = ""
if os.path.exists(man):
    with open(man, encoding="utf-8") as f:
        existing = f.read().rstrip()
block = existing + "\n"
for name, nframes, opts in created:
    block += f"""
Name: {name}
Min butthurt: 0
Max butthurt: 18
Min level: {opts['minlvl']}
Max level: {opts['maxlvl']}
Weight: {opts['weight']}
"""
with open(man, "w", newline="\n") as f:
    f.write(block.rstrip() + "\n")
print("manifest.txt aktualisiert:", man)
