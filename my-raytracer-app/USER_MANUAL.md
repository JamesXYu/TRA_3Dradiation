# Radiation 3D Frontend User Manual

## Table of Contents
1. [Overview](#overview)
2. [Interface Layout](#interface-layout)
3. [Floor Plan Management](#floor-plan-management)
4. [Plane Operations](#plane-operations)
5. [View Controls](#view-controls)
6. [Scaling and Measurement](#scaling-and-measurement)
7. [Precision Settings](#precision-settings)
8. [Save and Load Operations](#save-and-load-operations)
9. [Calculation Features](#calculation-features)
10. [Keyboard Shortcuts](#keyboard-shortcuts)
11. [Mouse Controls](#mouse-controls)
12. [Troubleshooting](#troubleshooting)

## Overview

The Radiation 3D Frontend is a web-based application for creating and analyzing 3D radiation scenarios. It allows users to upload floor plans, place radiation sources and receivers, and calculate heat flux distributions.

## Interface Layout

### Left Sidebar (Control Panel)
The main control panel is located on the left side of the screen with the following sections:

- **Floor Plan** - Image upload and management
- **Plane** - Object creation and manipulation
- **Views** - Camera angle controls
- **Scaling** - Distance measurement and calibration
- **Precision** - Grid resolution settings
- **Save** - Scene export/import
- **Calculation** - Analysis and visualization

### Main Viewport
The central 3D viewport displays:
- 3D scene with planes and objects
- Coordinate axes (X: red, Z: blue)
- Grid for reference
- Interactive camera controls

## Floor Plan Management

### Upload Image
1. Click **"Upload Image"** button
2. Select an image file (PNG, JPG, etc.)
3. The image will be displayed as a flat plane in the scene
4. Use this as a reference for placing radiation objects

**Supported formats:** PNG, JPG, JPEG, GIF, BMP

## Plane Operations

### Creating Planes
1. Click **"Add Plane"** button
2. A new plane (default: yellow receiver) will appear at the origin
3. Use transform controls to position and orient the plane

### Plane Types
- **Receiver** (Yellow) - Measures radiation/heat flux
- **Emitter** (Red) - Source of radiation/heat
- **Inert** (Blue) - Blocks or reflects radiation

### Selecting Planes
- **Single Selection:** Click on a plane
- **Multiple Selection:** Hold Shift and click additional planes
- **Deselect:** Click on empty space or press Delete

### Deleting Planes
1. Select the plane(s) to delete
2. Click **"Delete Plane"** button OR press Delete key
3. Works in all view modes (including top view)

### Resetting Planes
1. Select the plane(s) to reset
2. Click **"Reset Plane"** button
3. Returns planes to default position, size, and properties

### Plane Properties (GUI Panel)
When a plane is selected, the following properties can be modified:

- **Name** - Unique identifier for the plane
- **Width** - Horizontal dimension
- **Height** - Vertical dimension  
- **X-position** - Horizontal position
- **Y-position** - Vertical position (height)
- **Z-position** - Depth position
- **Rotation** - Y-axis rotation (0-360°)
- **Inclination** - X-axis tilt (-180° to +180°)
- **Heat Flux** - Temperature value (emitters only)
- **Type** - Receiver/Emitter/Inert
- **Receiver X-positive** - Normal direction for receivers

## View Controls

### Camera Views
- **Top View** - Looking down from above (bird's eye view)
- **Front View** - Looking from the front
- **Side View** - Looking from the side
- **Perspective View** - 3D perspective view

### View Switching Methods
1. **Button Controls:** Click the respective view buttons
2. **Keyboard Shortcuts:**
   - **T** - Top view
   - **Y** - Perspective view

### Top View Features
- Planes appear as colored wireframe lines
- Shows plane dimensions and orientation
- Optimized for precise positioning and measurement
- All plane operations work in this view

## Scaling and Measurement

### Setting Scale Reference
1. Click **"Set scale"** button
2. The view will switch to top view automatically
3. Click two points on the uploaded floor plan image
4. Enter the actual distance between these points
5. Click **"Update"** to apply the scale

### Scale Usage
- The scale factor is applied to all measurements
- Exported data will use real-world units
- Scale is saved with the scene file

## Precision Settings

### Receiver Grid Size
- **Input:** Number from 1 to 50
- **Default:** 10
- **Purpose:** Controls the resolution of heat flux calculations
- **Higher values:** More detailed but slower calculations
- **Lower values:** Faster but less detailed results

## Save and Load Operations

### Export Scene
1. Click **"Export Scene"** button
2. A JSON file will be downloaded
3. Contains all plane positions, properties, and settings
4. Includes scaling information if set

### Import Scene
1. Click **"Import Scene"** button
2. Select a previously exported JSON file
3. The scene will be restored exactly as saved
4. All planes, positions, and settings are preserved

## Calculation Features

### Calculate Heat Flux
1. Set up your scene with receivers and emitters
2. Adjust precision settings if needed
3. Click **"Calculate"** button
4. The system will:
   - Send scene data to the backend
   - Perform radiation calculations
   - Display results as contour plots

### Show Detail
1. Select a receiver plane
2. Click **"Show detail"** button
3. A modal window opens showing:
   - Heat flux contour visualization
   - Interactive zoom and pan
   - Hover to see exact values
   - Color-coded temperature distribution

### Contour Visualization
- **Zoom:** Mouse wheel to zoom in/out
- **Values:** Hover over points to see exact heat flux values
- **Colors:** Turbo color scheme (blue=cool, red=hot)
- **Resolution:** Based on precision settings

## Keyboard Shortcuts

### Transform Controls
- **W** - Switch to translate mode (move objects)
- **E** - Switch to scale mode (resize objects)
- **R** - Switch to rotate mode (rotate objects)
- **Q** - Toggle between local and world coordinate systems
- **G** - Toggle grid snapping on/off

### View Controls
- **T** - Switch to top view
- **Y** - Switch to perspective view

### Editing
- **Delete** - Delete selected objects
- **Ctrl+Z** - Undo last action
- **Ctrl+Y** - Redo last undone action
- **Ctrl+Shift+Z** - Alternative redo shortcut

### Copy/Paste
- **Ctrl+C** - Copy selected objects
- **Ctrl+V** - Paste copied objects

## Mouse Controls

### Camera Navigation
- **Left Click + Drag** - Rotate camera (perspective view)
- **Right Click + Drag** - Pan camera
- **Mouse Wheel** - Zoom in/out
- **Double Click** - Focus on object

### Object Manipulation
- **Left Click** - Select object
- **Shift + Left Click** - Add/remove from selection
- **Drag Transform Handles** - Move, rotate, or scale objects

### Transform Controls
When an object is selected, colored handles appear:
- **Red Handle** - X-axis manipulation
- **Green Handle** - Y-axis manipulation  
- **Blue Handle** - Z-axis manipulation
- **Yellow Circle** - Rotation (rotate mode)

## Advanced Features

### Multi-Object Selection
- Hold Shift while clicking to select multiple objects
- Selected objects can be moved together as a group
- Group operations maintain relative positions

### Grid Snapping
- Press **G** to toggle grid snapping
- Objects snap to 0.5-unit grid when moving
- Helps with precise alignment and positioning

### Coordinate Systems
- **Local Space** - Relative to object orientation
- **World Space** - Relative to scene coordinates
- Press **Q** to toggle between modes

### History System
- Full undo/redo support for all operations
- Maintains up to 500 history steps
- Includes plane creation, deletion, modification, and grouping

## Troubleshooting

### Common Issues

**Q: Can't see the uploaded floor plan image**
- Check that the image file is a supported format
- Ensure the image is not corrupted
- Try a different image file

**Q: Planes appear as wireframes in top view**
- This is normal behavior in top view
- Switch to perspective view to see full 3D planes
- All operations work the same in both views

**Q: Can't delete planes in top view**
- Make sure you're clicking on the colored wireframe line
- Ensure the plane is selected (highlighted)
- Try switching to perspective view first

**Q: Transform controls not working**
- Ensure an object is selected
- Check that you're in the correct transform mode (W/E/R)
- Try pressing Q to toggle coordinate systems

**Q: Calculation not working**
- Check that you have at least one receiver plane
- Ensure the backend server is running
- Verify internet connection for backend communication

**Q: Import/Export not working**
- Check browser console for error messages
- Ensure you have permission to download files
- Try using a different browser

### Performance Tips

- Use lower precision values (5-10) for faster calculations
- Limit the number of receiver planes for better performance
- Use top view for precise positioning to avoid camera issues
- Group related objects to reduce scene complexity

### Browser Compatibility
- **Recommended:** Chrome, Firefox, Edge (latest versions)
- **Required:** WebGL support enabled
- **Minimum:** 4GB RAM for complex scenes

## Technical Details

### Backend Communication
- Uses REST API for calculations
- Configurable backend URL in CONFIG object
- Automatic health checks and status monitoring
- Timeout handling for long calculations

### Data Format
- Scene files use JSON format
- Includes complete plane properties and relationships
- Backward compatible with previous versions
- Includes metadata for version tracking

### Coordinate System
- **X-axis:** Left/Right (red)
- **Y-axis:** Up/Down (green) 
- **Z-axis:** Forward/Back (blue)
- Right-handed coordinate system
- Origin at scene center

---

*This manual covers all major features of the Radiation 3D Frontend. For additional support or feature requests, please contact the development team.*