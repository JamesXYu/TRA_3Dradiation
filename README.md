# Radiation 3D Frontend

A web-based application for creating and analyzing 3D radiation scenarios. Upload floor plans, place radiation sources and receivers, and calculate heat flux distributions with interactive 3D visualization.

##Disclaimer
This tool is for professional use within OFR Consultants. Results are for design guidance only and must be verified by a qualified fire engineer. 
James Yu accepts no liability for any loss or error arising from its use.

## 🚀 Quick Start

1. **Upload Floor Plan**: Click "Upload Image" to add a reference floor plan
2. **Add Objects**: Click "Add Plane" to create radiation receivers, emitters, or inert surfaces
3. **Set Scale**: Use "Set scale" to calibrate measurements to real-world units
4. **Calculate**: Click "Calculate" to run radiation analysis and view results

## 📋 Table of Contents

- [Features](#features)
- [Interface Overview](#interface-overview)
- [Basic Operations](#basic-operations)
- [Keyboard Shortcuts](#keyboard-shortcuts)
- [Advanced Features](#advanced-features)
- [Troubleshooting](#troubleshooting)
- [Technical Details](#technical-details)

## ✨ Features

- **3D Visualization**: Interactive Three.js-based 3D scene with multiple camera views
- **Floor Plan Support**: Upload and scale reference images (PNG, JPG, etc.)
- **Object Management**: Create, position, and modify radiation objects
- **Real-time Calculations**: Backend integration for heat flux analysis
- **Contour Visualization**: Interactive temperature distribution plots
- **Scene Persistence**: Save and load complete scenarios
- **Multi-object Selection**: Group operations and batch editing
- **Undo/Redo System**: Full history support for all operations

## 🖥️ Interface Overview

### Control Panel (Left Sidebar)
- **Floor Plan** - Image upload and management
- **Plane** - Object creation and manipulation  
- **Views** - Camera angle controls (Top, Front, Side, Perspective)
- **Scaling** - Distance measurement and calibration
- **Precision** - Grid resolution settings (1-50)
- **Save** - Scene export/import functionality
- **Calculation** - Analysis and visualization tools

### Main Viewport
- 3D scene with interactive camera controls
- Coordinate axes (X: red, Z: blue)
- Grid reference system
- Transform controls for object manipulation

## 🎯 Basic Operations

### Creating and Managing Objects

**Add Plane**
```bash
Click "Add Plane" → New receiver appears at origin
```

**Object Types**
- 🟡 **Receiver** - Measures radiation/heat flux
- 🔴 **Emitter** - Source of radiation/heat  
- 🔵 **Inert** - Blocks or reflects radiation

**Selection Methods**
- Single: Click object
- Multiple: Shift + Click
- Deselect: Click empty space

### View Controls

**Camera Views**
- **Top View** (T) - Bird's eye view with wireframe objects
- **Front View** - Looking from front
- **Side View** - Looking from side  
- **Perspective View** (Y) - Full 3D perspective

### Scaling and Measurement

1. Click **"Set scale"** → Switches to top view
2. Click two points on floor plan image
3. Enter actual distance between points
4. Click **"Update"** → Scale applied to all measurements

### Object Properties

When selected, objects show these properties:
- **Name** - Unique identifier
- **Dimensions** - Width/Height
- **Position** - X/Y/Z coordinates
- **Rotation** - Y-axis rotation (0-360°)
- **Inclination** - X-axis tilt (-180° to +180°)
- **Heat Flux** - Temperature (emitters only)
- **Type** - Receiver/Emitter/Inert
- **Normal Direction** - Receiver orientation

## ⌨️ Keyboard Shortcuts

### Transform Controls
| Key | Action |
|-----|--------|
| `W` | Switch to translate mode (move) |
| `E` | Switch to scale mode (resize) |
| `R` | Switch to rotate mode |
| `Q` | Toggle local/world coordinate systems |
| `G` | Toggle grid snapping |

### View Controls
| Key | Action |
|-----|--------|
| `T` | Switch to top view |
| `Y` | Switch to perspective view |

### Editing
| Key | Action |
|-----|--------|
| `Delete` | Delete selected objects |
| `Ctrl+Z` | Undo |
| `Ctrl+Y` | Redo |
| `Ctrl+Shift+Z` | Alternative redo |

### Copy/Paste
| Key | Action |
|-----|--------|
| `Ctrl+C` | Copy selected objects |
| `Ctrl+V` | Paste copied objects |

## 🖱️ Mouse Controls

### Camera Navigation
- **Left Click + Drag** - Rotate camera (perspective view)
- **Right Click + Drag** - Pan camera
- **Mouse Wheel** - Zoom in/out
- **Double Click** - Focus on object

### Object Manipulation
- **Left Click** - Select object
- **Shift + Left Click** - Add/remove from selection
- **Drag Handles** - Move, rotate, or scale objects

### Transform Handles
- 🔴 **Red** - X-axis manipulation
- 🟢 **Green** - Y-axis manipulation
- 🔵 **Blue** - Z-axis manipulation
- 🟡 **Yellow Circle** - Rotation (rotate mode)

## 🔧 Advanced Features

### Multi-Object Selection
- Hold `Shift` while clicking to select multiple objects
- Selected objects move together as a group
- Maintains relative positions during group operations

### Grid Snapping
- Press `G` to toggle grid snapping
- Objects snap to 0.5-unit grid increments
- Helps with precise alignment

### Precision Settings
- **Receiver Grid Size**: 1-50 (default: 10)
- Higher values = more detailed but slower calculations
- Lower values = faster but less detailed results

### Scene Management
- **Export**: Saves complete scene as JSON file
- **Import**: Restores scene exactly as saved
- Includes all properties, positions, and scaling

### Calculation Features
1. Set up scene with receivers and emitters
2. Adjust precision settings if needed
3. Click **"Calculate"** → Sends data to backend
4. View results as interactive contour plots
5. Use **"Show detail"** for detailed analysis

## 🔍 Troubleshooting

### Common Issues

**Can't see uploaded floor plan**
- ✅ Check image format (PNG, JPG supported)
- ✅ Ensure image isn't corrupted
- ✅ Try different image file

**Planes appear as wireframes in top view**
- ✅ Normal behavior in top view
- ✅ Switch to perspective view for full 3D planes
- ✅ All operations work in both views

**Can't delete planes in top view**
- ✅ Click on colored wireframe line
- ✅ Ensure plane is selected (highlighted)
- ✅ Try switching to perspective view first

**Transform controls not working**
- ✅ Ensure object is selected
- ✅ Check correct transform mode (W/E/R)
- ✅ Try pressing Q to toggle coordinate systems

**Calculation not working**
- ✅ Ensure at least one receiver plane exists
- ✅ Check backend server is running
- ✅ Verify internet connection

### Performance Tips
- Use lower precision values (5-10) for faster calculations
- Limit receiver planes for better performance
- Use top view for precise positioning
- Group related objects to reduce complexity

### Browser Requirements
- **Recommended**: Chrome, Firefox, Edge (latest)
- **Required**: WebGL support enabled
- **Minimum**: 4GB RAM for complex scenes

## 🔧 Technical Details

### Backend Integration
- REST API for calculations
- Configurable backend URL
- Automatic health checks
- Timeout handling for long calculations

### Data Format
- Scene files: JSON format
- Complete plane properties and relationships
- Backward compatible
- Version tracking included

### Coordinate System
- **X-axis**: Left/Right (🔴 red)
- **Y-axis**: Up/Down (🟢 green)
- **Z-axis**: Forward/Back (🔵 blue)
- Right-handed coordinate system
- Origin at scene center

### Architecture
- **Frontend**: HTML5, Three.js, D3.js
- **3D Engine**: Three.js with WebGL
- **Visualization**: D3.js for contour plots
- **UI**: Custom CSS with Poppins font
- **State Management**: Custom history system

## 📦 Installation

1. Clone the repository
2. Open `index.html` in a modern web browser
3. Ensure backend server is running (if using calculations)
4. Start creating radiation scenarios!

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🆘 Support

For additional support or feature requests:
- Check the troubleshooting section above
- Review browser console for error messages
- Contact the development team

---

**Ready to start?** Open `index.html` in your browser and begin creating 3D radiation scenarios! 🌟
