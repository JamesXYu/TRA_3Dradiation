# Thermal Radiation Analysis - User Manual

## 📚 Table of Contents
- [Overview](#overview)
- [Interface Layout](#interface-layout)
- [Keyboard Shortcuts](#keyboard-shortcuts)
- [Mouse Controls](#mouse-controls)
- [Features by Section](#features-by-section)
- [Common Workflows](#common-workflows)
- [Tips & Best Practices](#tips--best-practices)

---

## 🎯 Overview

This application allows you to create 3D thermal radiation models, calculate radiation exchange between surfaces, and visualize temperature distributions on receiver planes.

**Key Concepts:**
- **Emitters**: Surfaces that emit thermal radiation (have temperature)
- **Receivers**: Surfaces that measure incoming radiation
- **Inert**: Non-participating surfaces (block radiation but don't emit/receive)

---

## 🖥️ Interface Layout

The interface consists of:
- **3D Viewport**: Main area showing your 3D model
- **Left Sidebar**: Collapsible sections with controls
- **Grid**: Reference grid (can be toggled on/off)
- **Coordinate System**: X (red), Y (green), Z (blue) axes

---

## ⌨️ Keyboard Shortcuts

### General
| Shortcut | Action |
|----------|--------|
| `Ctrl + Z` (or `Cmd + Z` on Mac) | Undo |
| `Ctrl + Y` (or `Ctrl + Shift + Z`) | Redo |
| `Delete` | Delete selected plane(s) |
| `Ctrl + C` | Copy selected plane(s) |
| `Ctrl + V` | Paste copied plane(s) |
| `Shift` (hold) | Multiple selection mode |

### Transform Controls
| Shortcut | Action |
|----------|--------|
| `Q` | Toggle local/global transform mode |
| `W` | Switch to translate (move) mode |
| `E` | Switch to scale mode |
| `R` | Switch to rotate mode |

### Modal Dialogs
| Shortcut | Action |
|----------|--------|
| `Enter` | Confirm input (in dialogs) |
| `Escape` | Cancel/close modal (in level name dialog) |

---

## 🖱️ Mouse Controls

### Basic Navigation
| Action | Control |
|--------|---------|
| **Pan** (orthographic views) | Left-click + drag |
| **Rotate** (perspective view) | Left-click + drag |
| **Zoom** | Scroll wheel |
| **Pan (alternative)** | Right-click + drag |

### Object Selection
| Action | Control |
|--------|---------|
| **Select plane** | Single click on plane |
| **Add to selection** | Hold `Shift` + click plane |
| **Deselect** | Click empty space |
| **Multi-select** | Hold `Shift` + click multiple planes |

### Top View Sphere Controls
| Action | Control |
|--------|---------|
| **Hover sphere** | Sphere turns white (indicates clickable) |
| **First click** | Click once - sphere pulses (ready) |
| **Double-click** | Click twice within 0.5s - sphere turns black (selected) |
| **Drag sphere** | After double-click, drag to adjust plane width/rotation |

### Wall Creation Mode
| Action | Control |
|--------|---------|
| **Add point** | Single click on ground |
| **Finish walls** | Double-click anywhere |
| **Continue** | Keep single-clicking to add more walls |

---

## 📋 Features by Section

### 🏠 Floor Plan
**Purpose**: Import and scale reference images

**Controls:**
- **Choose file**: Upload a floor plan image (PNG, JPG)
- **Input field**: Enter actual width/height of reference in meters

**Workflow:**
1. Click "Choose file" and select your floor plan image
2. Enter the real-world dimension
3. Image appears in 3D space as reference

---

### 📐 Plane

**Add** - Create new plane at origin (0, 0, 0)
- Default size: 1m × 1m
- Default type: Receiver
- Default color: Dark yellow

**Delete** - Remove selected plane(s)
- Keyboard shortcut: `Delete` key
- Removes from scene permanently

**Reset** - Reset plane to default values
- Position: (0, 0.5, 0)
- Size: 1m × 1m
- Rotation: 0°
- Color: Default color

**Stand** - Rotate plane to vertical position
- Quick way to create walls
- Sets incline to 90°

**Add Walls** - Continuous wall creation mode
1. Click to place first point
2. Click again to place second point (creates wall between points)
3. Keep clicking to add more connected walls
4. Double-click anywhere to finish
5. Button shows "Adding walls" when active

**Features:**
- Automatically creates vertical planes between points
- Each segment becomes a separate plane
- Names automatically: "Plane 1", "Plane 2", etc.
- All planes are 1m height by default

---

### 🎮 Controls

**Transform Modes:**

**Local/Global** (`Q` key)
- **Local**: Transforms relative to object's rotation
- **Global**: Transforms relative to world axes

**Translate** (`W` key)
- Move plane in 3D space
- Drag colored arrows (X/Y/Z axes)
- Or drag center to move freely

**Scale** (`E` key)
- Resize plane
- Drag colored squares to scale on specific axes
- Or drag center to scale uniformly
- On top view, double click the edge to enable dragging the plane to stretch

**Rotate** (`R` key)
- Rotate plane around axes
- Drag colored circles to rotate
- Or drag center sphere for free rotation

**Visual Indicators:**
- Selected plane turns grey
- Transform gizmo appears on selected plane
- Gizmo colors: Red (X), Green (Y), Blue (Z)

---

### 👁️ Views

**Camera Views:**

**Top** - Orthographic top-down view
- Shows wireframe lines for planes
- Enables sphere controls for width adjustment
- Best for layout and positioning

**Front** - Orthographic front view
- Side elevation view
- Good for checking heights and vertical alignment

**Side** - Orthographic side view
- Left elevation view
- Useful for checking depth positioning

**Perspective** - 3D perspective view
- Full 3D navigation
- Best for overall visualization
- Can rotate camera freely

**Toggle Grid** - Show/hide reference grid
- Grid helps with alignment
- Can toggle on/off for cleaner view

---

### 📏 Scaling

**Set Scale** - Calibrate real-world distances

**Workflow:**
1. Click "Set Scale"
2. Click first point on a known distance
3. Click second point to complete measurement
4. Enter actual distance in meters in the popup
5. Scene scales accordingly

**Use Cases:**
- Scale imported floor plans
- Match reference dimensions
- Verify measurements

---

### 🎯 Precision

**Translation/Rotation/Scaling Snap Settings:**

Each has:
- **Enable checkbox**: Turn snap on/off
- **Size input**: Snap increment value

**Translation Snap:**
- Default: 0.1 meters
- Snaps movement to grid increments

**Rotation Snap:**
- Default: 10 degrees
- Snaps rotation to angle increments

**Scaling Snap:**
- Default: 0.1 units
- Snaps scale to size increments

**Tips:**
- Enable for precise alignment
- Disable for free-form editing
- Adjust snap size as needed

---

### 🎨 Color Scale

**Purpose**: Customize temperature visualization colors

**Controls:**
- **Min Value**: Temperature for coldest color (blue)
- **Max Value**: Temperature for hottest color (red)
- **Apply**: Update color scale

**Color Gradient:**
- Blue (cold) → Green → Yellow → Orange → Red (hot)

**Use:**
- Set before or after calculation
- Affects contour visualization on receiver planes
- Does not affect actual calculations

---

### 💾 Save

**Export Scene** - Save current model to JSON file
- Saves all planes (position, size, rotation, type, temperature)
- Saves floor plan reference
- Saves scaling settings
- Can be imported later

**Note**: Automatically exports multi-level if you have saved levels

---

### 🧮 Calculation

**Show detail** - Display calculation data as contour plot
- Opens modal with temperature visualization
- Shows contour lines and color map
- Only works on receiver planes after calculation

**Calculate** - Run thermal radiation analysis
1. Validates scene (needs at least 1 receiver and 1 emitter)
2. Sends geometry to backend server
3. Calculates view factors and radiation exchange
4. Returns temperature distribution
5. Applies results as textures on receiver planes

**Requirements:**
- At least one Receiver plane
- At least one Emitter plane (with temperature > 0)
- Backend server must be running

**Output:**
- Temperature contours on receiver planes
- Values logged to console
- Can view detailed plots with "Show detail"

---

### 📦 Level Manager

**Purpose**: Manage multiple scenes/levels

**Save Level**
- Click "Save Level"
- Enter level name in dialog
- Click "Save Level" or press `Enter` to confirm
- Click "Cancel" or press `Escape` to cancel
- Warns if name already exists (can overwrite)

**Dropdown Menu**
- Lists all saved levels
- Select to prepare for loading

**Load**
- Select level from dropdown
- Click "Load"
- Current scene is replaced with saved level
- Warns before overwriting unsaved changes

**Delete**
- Select level from dropdown
- Click "Delete"
- Permanently removes saved level
- Requires confirmation

**New Level**
- Clears current scene
- Creates blank canvas
- Warns before clearing unsaved work

**Storage:**
- Levels saved in browser localStorage
- Persists between sessions
- Includes timestamp

---

## 🔄 Common Workflows

### Creating a Simple Model

1. **Add emitter plane:**
   - Click "Add" in Plane section
   - Select plane and use Properties panel
   - Set Type: "Emitter"
   - Set Temperature: e.g., 500 K

2. **Add receiver plane:**
   - Click "Add" again
   - Move it away from emitter (use `W` for translate)
   - Keep Type: "Receiver"

3. **Calculate:**
   - Click "Calculate" in Calculation section
   - Wait for results
   - View temperature distribution on receiver

---

### Using Floor Plan Reference

1. **Import image:**
   - Click "Choose file" in Floor Plan
   - Select your floor plan image

2. **Scale it:**
   - Enter known dimension (e.g., if room is 5m wide, enter "5")
   - Plan scales to correct size

3. **Create walls:**
   - Click "Add Walls"
   - Switch to Top view
   - Click corners to create walls
   - Double-click to finish

4. **Adjust wall heights:**
   - Select walls
   - Use Properties panel to set Height (e.g., 3m)

---

### Adjusting Plane Width in Top View

1. **Switch to Top view**
2. **Select plane** - wireframe with spheres appears
3. **Hover over sphere** - turns white
4. **Double-click sphere** - turns black (selected)
5. **Drag sphere** - adjusts plane width and rotation
6. **Click elsewhere** - deselect sphere

**Benefits:**
- Visual, intuitive width control
- Simultaneously adjusts rotation
- See changes in real-time

---

### Working with Multiple Planes

1. **Multi-select:**
   - Hold `Shift`
   - Click multiple planes
   - All turn grey

2. **Grouped operations:**
   - Set Height: affects all selected
   - Set Type: affects all selected
   - Transform: move/rotate all together
   - Delete: removes all selected

3. **Copy-paste:**
   - Select plane(s)
   - Press `Ctrl + C` to copy
   - Press `Ctrl + V` to paste
   - Pasted planes appear at same location (move them)

---

### Setting Up a Room

1. **Import floor plan** (if available)
2. **Create walls:**
   - Use "Add Walls" in top view
   - OR manually add planes and rotate to vertical
3. **Add ceiling:**
   - Add plane
   - Position at room height
   - Scale to room size
4. **Add floor:**
   - Add plane
   - Keep horizontal
   - Position at ground level
5. **Set types:**
   - Walls: Usually "Inert"
   - Heater/radiator: "Emitter" with temperature
   - Measurement area: "Receiver"

---

## 💡 Tips & Best Practices

### General Tips

✅ **Save frequently**
- Use Level Manager to save versions
- Export Scene for backup files

✅ **Use Top view for layout**
- Easier to see floor plan
- Sphere controls for precise width adjustment

✅ **Name your levels meaningfully**
- "Room_v1", "Kitchen_final", etc.
- Helps track iterations

✅ **Start simple**
- Test with 2-3 planes first
- Add complexity gradually

---

### Modeling Tips

✅ **Use consistent units**
- Everything is in meters
- Temperature in Kelvin

✅ **Check plane orientation**
- Receiver planes should face emitters
- Use normal arrow (green arrow) to verify direction
- Toggle normal direction with "Positive/Negative" if needed

✅ **Avoid overlapping planes**
- Can cause calculation errors
- Use slight offsets if needed

✅ **Group related planes**
- Use multi-select for walls, ceiling, etc.
- Set properties together

---

### Performance Tips

✅ **Minimize plane count**
- Use larger planes where possible
- Each plane increases calculation time

✅ **Appropriate grid resolution**
- Default N=5 is good starting point
- Increase for more detail, decrease for speed
- Set in Precision section before calculation

✅ **Save before calculating**
- Large calculations can take time
- Save to avoid losing work

---

### Calculation Tips

✅ **Verify requirements before Calculate:**
- At least 1 Receiver plane
- At least 1 Emitter plane
- Emitter temperature > 0

✅ **Check backend connection**
- Look for "Backend is healthy" in console
- If not, restart server: `./run.sh restart`

✅ **Interpreting results:**
- Contours show temperature distribution
- Blue = cold, Red = hot
- Uniform color = uniform radiation
- Gradients = radiation variation

✅ **Adjust color scale:**
- Set min/max in Color Scale section
- Better contrast for small temperature ranges

---

### Troubleshooting

❌ **"No receiver planes found"**
- Add at least one plane with Type = "Receiver"

❌ **"No emitter polygons found"**
- Add at least one plane with Type = "Emitter"
- Set Temperature > 0 on emitter

❌ **Calculation fails silently**
- Check browser console (F12) for errors
- Verify backend is running: `./run.sh status`

❌ **Can't see spheres in Top view**
- Spheres only appear in Top view
- Check you're not in Front/Side/Perspective view

❌ **Plane disappears after transform**
- May have moved far from origin
- Reset plane or use Undo (`Ctrl + Z`)

❌ **Undo not working**
- History is cleared on scene load/import
- Save before major operations

---

## 📞 Quick Reference Card

**Most Common Shortcuts:**
```
Ctrl + Z        Undo
Ctrl + Y        Redo
Delete          Delete plane
W               Move tool
E               Scale tool
R               Rotate tool
Shift + Click   Multi-select
Ctrl + C/V      Copy/Paste
```

---

## 📖 Related Documentation

- **README.md**: Installation and setup guide (ignore)
- **Browser Console** (F12): Detailed calculation logs and error messages
- **Backend**: See `backend/server.cpp` for calculation details

---

**Version**: 1.0  
**Last Updated**: 2025

For technical support or bug reports, check the project repository or contact your system administrator.

