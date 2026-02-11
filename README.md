# ascii_graph
Terminal-based ASCII graph coordinate plotter (first quadrant) written in C


# ASCII Graph – Coordinate Plotter in C

This is a **terminal-based ASCII graphing program** written in **C**, designed to plot
2D coordinate points in the **first quadrant only**.

The program renders a graph using plain text characters, automatically scales
input values to fit the grid, and visually represents overlapping points.

This project was built as a **learning-focused, low-level programming exercise**
rather than a polished plotting library.

---

## Features (User Perspective)

- Plot 2D coordinate points in the terminal
- Supports **multiple points** at once
- Automatically scales the graph to fit large inputs
- Two scaling modes:
  - **Independent scaling** (X and Y scaled separately)
  - **Uniform scaling** (preserves shape)
- Clearly marked axes and origin
- Indicates when **multiple points overlap** in the same cell
- Input validation with re-prompting on invalid entries

---

## Features (Developer / Technical Perspective)

- Implemented entirely in **C (C17 compatible)**
- Uses a fixed-size 2D character array as a rendering canvas
- Manual coordinate-to-grid mapping logic
- Explicit handling of:
  - Scaling tradeoffs
  - Rounding behavior
  - Finite grid resolution limitations
- Defensive input handling using `scanf` return values
- No external libraries beyond `stdio.h`
- Designed with extensibility in mind (scaling logic, plotting logic, validation)

---


Known Limitations

-Only supports the first quadrant (no negative coordinates)
-Extreme values may still cause visual crowding due to finite grid size
-Out-of-bounds protection for all edge cases is documented but not fully enforced
-ASCII resolution limits precision (by design)
-These limitations are acknowledged intentionally and documented in code comments.

---

Acknowledgement

This project was fully written by me, including all logic, structure,
and implementation decisions.
I used ChatGPT as a learning aid only for:
-Clarifying C language behavior (e.g., input handling, edge cases)
-Discussing design tradeoffs (scaling strategies, rounding effects)
-Reviewing logic at a conceptual level
No code was generated or copied directly.
All implementation decisions and final code were authored by me.

