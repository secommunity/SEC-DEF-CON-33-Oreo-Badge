# SEC O.R.E.O. Badge – B.A.D.G.E. Compliance Sheet

This page provides a detailed overview of the SEC O.R.E.O. Badge's compliance with the **B.A.D.G.E. framework** (Badge Accountability & Disclosure for General Ethics). The goal is to transparently share safety, environmental, and ethical design decisions.

✅ = Fully compliant  
❌ = Not compliant  
⬜ = Not applicable to this badge  

---

## Compliance Checklist

| Category                        | Item                                                | Status | Notes                            |
|--------------------------------|-----------------------------------------------------|--------|-----------------------------------|
| **Human Safety**               |                                                     |        |                                   |
| **Battery & Power Safety**      | Overcharge protection                               | ✅     | TI BQ25895 onboard and built into battery  |
|                                 | Current-limited charging                            | ✅     | Limited by TI BQ25895 config      |
|                                 | Temperature protection                              | ✅     | Built into battery                |
|                                 | Short-circuit protection                            | ✅     | Built into battery                |
|                                 | Battery physically covered                          | ❌     |                                   |
|                                 | Reverse polarity protection                         | ✅     | Built into battery                |
|                                 | Battery securely adhered                            | ✅     | 4mm adhesive foam                 |
|                                 | Battery brand and chemistry disclosed               | ✅     | YDL 3.7v 3000mA LiPo              |
| **User Safety & EMC**           | No exposed high-voltage pads                        | ✅     | Max voltage = 5V                 |
|                                 | Exposed conductive surfaces minimized               | ✅     | No exposed copper; all pads covered by soldermask; nylon fasteners |
|                                 | Non-conductive housing or enclosure                 | ✅     |                                   |
|                                 | PCB edges rounded or beveled                        | ✅     | Rounded corners                   |
|                                 | Sharp component leads trimmed or insulated          | ✅     |                                   |
|                                 | Battery connectors not user-accessible              | ❌     |                                   |
|                                 | All external ports labeled                          | ✅     |                                   |
|                                 | Heat-tested under continuous use                    | ✅     | Stable under 6+ hrs at max load   |
|                                 | Low-voltage toy-safe design (<24V)                  | ✅     | RP2040 logic                      |
| **Connectivity & RF**           | No wireless (Wi-Fi/BLE/RF)                          | ✅     | No radio components               |
|                                 | USB data lines disabled                             | ❌     | USB only used for charging and reprogramming |
|                                 | No HID, keyboard, or storage emulation              | ✅     | Storage emulation only for firmware loading in RP2040 bootloader |
|                                 | Use of off-the-shelf RF modules / ICs               | ⬜     | Not applicable – no RF present    |
|                                 | RF emissions within FCC limits                      | ⬜     | Not applicable – no transmitters  |
|                                 | RF exposure safe for wearable devices               | ⬜     | Not applicable – no RF hardware   |
|                                 | RF modules have shielding cans                      | ⬜     | Not applicable – no RF hardware   |
|                                 | MAC address randomized or disclosed                 | ⬜     | Not applicable - no network       |
|                                 | Transmission duty cycle limited                     | ⬜     | Not applicable – no RF present    |
| **Privacy**                     | No user data stored                                 | ✅     | No flash usage                    |
|                                 | Firmware is open source                             | ✅     |                                   |
|                                 | No persistent user identifiers                      | ✅     |                                   |
|                                 | No telemetry or beaconing                           | ✅     |                                   |
|                                 | QR codes or NFC tags disclosed                      | ✅     |                                   |
| **Environmental Impact**        |                                                     |        |                                   |
| **Materials & Manufacturing**   | RoHS-compliant components                           | ✅     | JLCPCB RoHS BOM                   |
|                                 | REACH-compliant where possible                      | ✅     | Common passive parts              |
|                                 | Lead-free solder used                               | ✅     | SAC305 alloy                      |
|                                 | ENIG finish (no leaded HASL)                        | ✅     | Yes                               |
|                                 | Halogen-free PCB laminate                           | ✅     |                                   |
|                                 | Low-VOC silkscreen ink                              | ✅     |                                   |
|                                 | Conflict minerals sourcing disclosure               | ❌     |                                   |
|                                 | Low-layer-count PCB                                 | ✅     |                                   |
| **Design for Reuse / Repair**   | Badge can be repurposed post-event                 | ✅     | Arduino-compatible firmware       |
|                                 | Modular parts/connectors for reuse                  | ✅     | USB-C, JST-PH                    |
|                                 | Minimal adhesives used                              | ✅     | No glues, only removable tape     |
|                                 | Designed for easy disassembly                      | ✅     | Screws, no welding                |
|                                 | Example code for reuse provided                     | ✅     |                                   |
|                                 | Pinout diagram included                             | ⬜     | Not applicable - no user exposed headers |
|                                 | OpenSCAD/STEP files for enclosure shared            | ❌     |                                   |
| **End-of-Life Responsibility**  | Battery is removable                                | ✅     | JST-PH connector                  |
|                                 | Disposal guidance provided                          | ❌     |                                   |
|                                 | Plastic marked or recyclable                        | ✅     | PLA shell                         |
|                                 | Battery removal instructions included               | ✅     |                                   |
|                                 | Silkscreen includes recycle/warning icons           | ❌     |                                   |
|                                 | Can be returned, reused, or donated                 | ✅     | Encouraged at next DEF CON        |
| **Supply Chain and Transparency** | BOM and schematic published                        | ❌     |                                   |
|                                 | Traceable part sourcing                             | ✅     | LCSC, DigiKey BOM                 |
|                                 | Local or regional manufacturing preferred           | ❌     | JLCPCB (China)                    |
|                                 | Ethical material sourcing where possible            | ✅     | Lead-free parts and supply chain  |
|                                 | GitHub includes changelog or commit history         | ✅     |                                   |

---

Want to use this format for your own badge? Visit the [B.A.D.G.E. Framework](https://github.com/TheDukeZip/B.A.D.G.E.-Framework/) and start documenting with transparency.
