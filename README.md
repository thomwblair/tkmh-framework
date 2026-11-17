# Topological Kinematics and Chiral Flow (TKMH)

> **Standard Proposal for Interoperable Character Rigging & GPU Compute Runtimes**
> **Version:** 1.0.0-rc.1

---

## What is TKMH?

Traditional character rigging relies heavily on **Forward Kinematics (FK)** and **Inverse Kinematics (IK)**. While effective for basic movement, these legacy approaches often suffer from mechanical stiffness, gimbal lock, and uncorrected temporal drift during complex, organic motions.

**Topological Kinematics and Chiral Flow (TKMH)** introduces an advanced paradigm that treats skeletal structures as continuous topological manifolds. By combining **3.5-turn chiral spinal winding ($7\pi h$)**, **unit dual quaternion interpolation ($\mathbb{H}_D$)**, and **strict Frobenius norm manifold clamping ($\kappa \le 0.20$)**, TKMH automates organic counter-torque and eliminates temporal drift in real-time engines and DCC pipelines.

---

## Key Innovations

* **Chiral Spinal Winding ($\theta(h)$):** Parameterizes vertebral spans from pelvic root ($h = 0.0$) to cranial apex ($h = 1.0$), ensuring an exact $180^\circ$ counter-torque inversion ($7\pi \equiv \pi \pmod{2\pi}$).
* **Manifold Invariant Tensor Hashing:** Enforces a strict Frobenius norm boundary threshold ($\kappa \le 0.20$) to prevent catastrophic pose spikes and inter-frame drift.
* **Dual-Quaternion Lie Group Mappings:** Eliminates gimbal lock and dual-axis pinching by evaluating shortest-path geodesics on the unit hypersphere $\text{SU}(2)$.
* **Wu Wei Equilibrium:** Integrates an energy-minimized postural solver to keep center-of-mass projections stably within the support polygon.

---

## Repository Structure

This package is structured for immediate integration across studio pipelines and runtime engines:

```text
Topological Kinematics and Chiral Flow/
├── tkmh_normative_spec.md    # 5-section master normative specification (RFC 2119)
├── tkmh_parser.cpp           # Production-ready C++ parser and container validator
├── tkmh_schema.usda          # Pixar OpenUSD schema extension for DCC pipelines
└── validate_chiral.py        # Python test bench for .chiral binary header verification
```

---

## Quick Start & Verification

### 1. Validate a `.chiral` Binary Container (Python)
You can use the provided Python script to inspect container headers, verify magic signatures (`CHRL`), and check offset bounds:
```bash
python validate_chiral.py sample_asset.chiral
```

### 2. Compile and Run the C++ Parser
The C++ parser enforces memory safety and strict bounds checking on binary payloads:
```bash
g++ -std=c++17 tkmh_parser.cpp -o tkmh_parser
./tkmh_parser sample_asset.chiral
```

### 3. OpenUSD Studio Integration
Reference `tkmh_schema.usda` inside your asset sublayers to expose `TKMHSpineAPI` properties directly within Unreal Engine, Houdini, or Maya.

---

## License & Conformance
This specification is released under standard open-source interoperability guidelines. Compliant runtimes **MUST** adhere to the normative requirements defined in `tkmh_normative_spec.md`.
