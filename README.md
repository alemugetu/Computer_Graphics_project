#  OpenGL Computer Graphics Project

A collection of 2D graphics rendered using **OpenGL** and **FreeGLUT**, including an animated American flag and the Meta logo — built as part of a Computer Graphics course assignment.

---

##  Project Structure

```
├── meta_logo/
│   └── meta_logo_filled.cpp       # Filled Meta infinity logo (OpenGL + stencil buffer)
├── us_flag/
│   └── us_flag.cpp                # Animated waving US flag (OpenGL + GLUT timer)
└── README.md
```

---

##  Projects

### 1. Meta Logo

Renders the Meta (∞) infinity logo using cubic Bézier curves and OpenGL's stencil buffer to produce a correctly filled shape in the official Meta brand blue `RGB(0, 129, 251)`.

**Key techniques:**

- Cubic Bézier curve sampling from SVG path data
- Stencil buffer even-odd fill for non-convex shapes
- Three-pass render: outer fill → left hole → right hole

---

### 2. Animated US Flag

Renders a waving American flag with 13 stripes, a blue canton with 50 stars, a flagpole, and a smooth sine-wave animation driven by a GLUT timer.

**Key techniques:**

- Sine-wave vertex displacement (`GL_QUADS` strip)
- 5-point star geometry via `GL_TRIANGLE_FAN`
- Animated border with `GL_LINE_STRIP`
- GLUT timer loop at ~60 fps

---

##  Requirements

| Tool         | Version                    |
| ------------ | -------------------------- |
| C++ compiler | GCC / G++ (C++11 or later) |
| OpenGL       | 2.1+                       |
| FreeGLUT     | 3.x                        |

### Install FreeGLUT

**Ubuntu / Debian:**

```bash
sudo apt-get update
sudo apt-get install freeglut3-dev
```

**Windows:**
Download from [freeglut.sourceforge.net](http://freeglut.sourceforge.net/) and link via your IDE.

**macOS:**

```bash
brew install freeglut
```

---

##  How to Run

### Meta Logo

```bash
# Compile
g++ meta_logo/meta_logo_filled.cpp -o meta_logo -lGL -lGLU -lglut

# Run
./meta_logo
```

### Animated US Flag

```bash
# Compile
g++ us_flag/us_flag.cpp -o us_flag -lGL -lGLU -lglut

# Run
./us_flag
```

> **Windows note:** Replace `-lGL -lGLU -lglut` with `-lopengl32 -lglu32 -lfreeglut` and ensure `freeglut.dll` is in the same folder as the executable.

---

## 👥 Group Members & Contributions

| Name            | Student ID | Contributions               |
| --------------- | ---------- | --------------------------- |
| Alemu Getu      | 01821/16   | US flag stripes             |
| Amanuel Muluken | 01811/16   | Stencil Buffer + Canton     |
| Dagmawit Mesfn  | 02065/16   | Meta Bézier Curves          |
| Abel Mulu       | 01825/16   | Star Geometry               |
| Nepthalem Ayele | 00363/16   | Flagpole + Color Correction |
| Helen Brhanu    | 01955/16   | Wave animation              |

> Each member commits their own work directly to this repository. See the **Commits** tab for individual contribution history.

---

##  Notes

- All coordinate systems use **SVG-style top-left origin**, mapped to OpenGL via `gluOrtho2D` with an inverted Y-axis.
- The stencil buffer (`GLUT_STENCIL`) must be requested in `glutInitDisplayMode` for the Meta logo fill to work correctly.
- Wave animation runs at approximately **60 fps** using `glutTimerFunc(16, timer, 0)`.

---

##  License

This project is submitted for academic purposes only.
