# AnSoGicLabs // Native Desktop RPA Engine
### High-Performance Automation Framework for Legacy and Enterprise Systems
<img src="https://shields.io_|_Linux-blue" alt="Platform" />
<img src="https://shields.io" alt="Language" />
<img src="https://shields.io" alt="License" />

---

## 📋 Executive Summary
Modern enterprise workflows often rely on **legacy systems, desktop applications (Win32, Qt, Delphi, FoxPro), and terminal emulators** that completely lack modern APIs or web interfaces. This architectural limitation forces businesses to dedicate valuable human capital to repetitive data entry, introducing high error rates and severe operational bottlenecks.

This project is a production-ready **Proof of Concept (PoC) of a Lightweight, Non-Invasive Robotic Process Automation (RPA) Engine** written natively in **C++ and Qt**. By operating directly at the OS subsystem level, this architecture bypasses the need for dedicated APIs, allowing headless data orchestration, coordinate-targeted UI interactions, and screen-scraping pipelines with an ultra-low memory footprint (< 30MB RAM).

---

## 🛠️ Key Architectural Strengths

* **Asynchronous Multi-Threaded Pipeline:** Separates the core automation loops and hardware injection layer (`AutomationWorker`) from the Main UI Thread via `QThread`, preventing UI freezing and ensuring deterministic execution during complex data entry cycles.
* **Low-Level Native Input Injection:** Utilizes platform-specific sub-systems (such as the Win32 `SendInput` API for Windows and low-level event emulators for Linux) to bypass standard software abstractions and type text natively into protected industrial applications.
* **Targeted Computer Vision / Screen Scraping:** Instead of capturing massive full-screen buffers, the engine clips specific Regions of Interest (ROI) dynamically via `QScreen::grabWindow`, significantly lowering CPU and memory usage before sending frame buffers to OCR engines or OpenCV pipelines.
* **Cross-Platform Extensibility:** Built on top of a modular design capable of scaling from local Windows environments to highly secured Linux infrastructures (including modern Wayland environments utilizing custom portals/PipeWire interfaces).

---

## 🚀 Core Functionality Demo

### 1. Headless Data Entry & Input Emulation
The engine translates abstract text structures and Cartesian coordinates into low-level keyboard scans and mouse event flags. It implements controlled latencies between actions to perfectly mirror human interactions, avoiding inputs from being rejected by legacy application buffers:

```cpp
// Sample execution flow for zero-API Data Entry
INPUT inputs[3] = {};
// 1. Position and trigger mouse event precisely over target desktop coordinates
inputs[0].type = INPUT_MOUSE;
inputs[0].mi.dx = calculatedAbsX;
inputs[0].mi.dy = calculatedAbsY;
inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
// ... trigger left mouse button down/up event ...
::SendInput(3, inputs, sizeof(INPUT));

// 2. Direct Unicode Stream Typing into active field
for (const QChar &ch : textToType) {
    INPUT keyInput = {};
    keyInput.type = INPUT_KEYBOARD;
    keyInput.ki.wScan = ch.unicode();
    keyInput.ki.dwFlags = KEYEVENTF_UNICODE;
    ::SendInput(1, &keyInput, sizeof(INPUT));
}
```

### 2. High-Performance Region-of-Interest (ROI) Extraction
Perfectly suited for integrations with OCR (such as *Tesseract*) or computer vision frameworks (*OpenCV*) to cross-verify database fields against legacy on-screen UI outputs:

```cpp
// Low-overhead screenshot targeting a specific layout coordinate bounding box
QScreen *screen = QGuiApplication::primaryScreen();
QPixmap criticalZone = screen->grabWindow(0, roi.x(), roi.y(), roi.width(), roi.height());
emit screenProcessed(criticalZone);
```

---

## 🏢 Enterprise Business Use Cases

1. **Legacy ERP Data Synchronization:** Bridges the gap between modern cloud-based E-commerce APIs and older, on-premise local inventory management software by replicating data entry workflows overnight.
2. **Automated Financial Compliance & Audits:** Headless scraping of transaction data directly from legacy banking terminals, desktop software, or PDF viewers, generating standardized JSON/CSV output streams automatically.
3. **Industrial SCADA & IoT Gateway Control:** Interacting with human-machine interfaces (HMIs) or manufacturing control programs that cannot be modified due to strict vendor licensing or certifications.

---

## 🔧 Building and Prerequisites

### Requirements
* **C++ Compiler:** MSVC 2019+ (Windows) / GCC 9+ (Linux)
* **Framework:** Qt 5.15.x / Qt 6.x (Core, Gui, Widgets modules)
* **Build System:** CMake 3.16+ or QMake

### Compilation (CMake)
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

---

## 👤 About the Author

A senior systems and application developer with over **23 years of experience** building high-performance architectures, low-level operating system wrappers, and cross-platform native software using **C, C++, Assembly, Java, and Qt**. Specialist in reverse engineering desktop interfaces, memory optimization, and enterprise-grade robotic workflow automation.

For corporate inquiries, system integration consulting, or legacy automation projects, feel free to open an issue or connect directly through this profile.

