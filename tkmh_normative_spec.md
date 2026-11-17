# Topological Kinematics and Chiral Flow (TKMH) Normative Specification

**Document Version:** 1.0.0-rc.1  
**Status:** Standard Proposal for Interoperable Character Rigging & GPU Compute Runtimes  

---

## Section 1: Normative Conformance & Terminology

### 1.1 Scope
This specification defines the normative requirements for the Topological Kinematics and Chiral Flow (TKMH) framework. It governs mathematical invariants, data schema serialization (`.chiral` containers), GPU compute shader execution contracts, and conformance validation protocols for real-time engines and digital content creation (DCC) tools.

### 1.2 RFC 2119 Compliance Keywords
The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in this document are to be interpreted as described in RFC 2119.

### 1.3 Terminology
* **Topological Skeletal Tensor ($\mathcal{T}$):** An invariant matrix encoding the spatial-rotational state of a character rig across frames to prevent temporal drift.
* **Chiral Spinal Winding $\theta(h)$:** The half-integer torsional winding function ($7\pi h + \phi_0$) mapping normalized vertebral height to organic counter-torque.
* **Manifold Invariant Limit ($\kappa$):** The strict Frobenius norm boundary threshold governing permissible inter-frame tensor change.
* **Wu Wei Equilibrium:** An energy-minimized postural solver ensuring center-of-mass projection remains within the support polygon.

---

## Section 2: Mathematical Foundation

### 2.1 Spinorial Lie Group Mappings
* **Rotation Representation:** Runtimes MUST utilize unit dual quaternions $\hat{q} \in \mathbb{H}^D$ for joint transformations to eliminate gimbal lock and dual-axis pinching.
* **Double-Cover Alignment:** Rotational interpolation MUST evaluate shortest-path geodesics on the unit hypersphere $\mathrm{SU}(2)$.

### 2.2 The 3.5-Turn Chiral Spinal Winding
* **Normalized Domain:** The vertebral span MUST be parameterized across a normalized height variable $h \in [0, 1]$, where $h = 0.0$ represents the pelvic root and $h = 1.0$ represents the cranial apex.
* **Winding Equation:** The cumulative torsional rotation angle $\theta(h)$ MUST satisfy:
  $$\theta(h) = 3.5 \times 2\pi \cdot h + \phi_0 = 7\pi h + \phi_0$$
  where $\phi_0$ is the dynamic phase offset.
* **Inversion Guarantee:** Compliant solvers MUST ensure an exact $180^\circ$ counter-torque inversion between apex and root:
  $$\theta(1) - \theta(0) = 7\pi \equiv \pi \pmod{2\pi}$$

### 2.3 Manifold Invariant Tensor Hashing
* **Tensor Construction:** The frame tensor $\mathcal{T}$ MUST be computed via:
  $$\mathcal{T} = \sum_{i=1}^{N} w_i \left( \hat{q}_i \otimes (\mathbf{p}(p)_i - \mathbf{p}(parent)_i) \right)$$
* **Frobenius Norm Clamping:** Inter-frame variation MUST satisfy the invariant boundary limit $\kappa \le 0.20$:
  $$\left\|\mathcal{T}_{t+\Delta t} - \mathcal{T}_t\right\|_F \le \kappa$$
  If the computed delta exceeds $\kappa$, the runtime MUST trigger a damped relaxation pass.

---

## Section 3: Data Serialization & The `.chiral` Container

### 3.1 Binary Layout Specification
Files ending in the `.chiral` extension MUST be structured as a zero-overhead, little-endian binary format consisting of a fixed 44-byte header followed by tightly packed tensor arrays.

| Offset (Bytes) | Type | Field Name | Description |
| :--- | :--- | :--- | :--- |
| `0x00` - `0x03` | `char[4]` | Magic Bytes | Ascii signature: `TKMH` |
| `0x04` - `0x07` | `uint32_t` | Version | Format version (`0x00010000` for v1.0) |
| `0x08` - `0x1F` | `float[6]` | Manifold Bounds | Bounding box limits ($\min_x, \min_y, \min_z, \max_x, \max_y, \max_z$) |
| `0x20` - `0x23` | `float` | Clamping Limit ($\kappa$) | Invariant boundary threshold (default `0.20`) |
| `0x24` - `0x2B` | `uint64_t` | Bone Count & Flags | Lower 32 bits: Joint count $N$; Upper 32 bits: Feature flags |

---

## Section 4: GPU Compute Shader Contracts

### 4.1 HLSL / GLSL Binding Requirements
* Compute shaders implementing TKMH solvers MUST execute within a threadgroup size of `256 x 1 x 1`.
* Buffers allocated for structural skeletal tensors MUST use structured buffer alignment matching 16-byte register boundaries.

---

## Section 5: Conformance & Validation Protocol

### 5.1 Test Suite Requirements
* Implementations claiming conformance with TKMH v1.0.0-rc.1 MUST pass the reference test suite provided in `validate_chiral.py`.
* Any runtime producing a Frobenius norm divergence greater than $\kappa = 0.20$ under standard benchmark loads SHALL be classified as non-compliant.
