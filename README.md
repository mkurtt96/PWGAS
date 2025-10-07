# ⚡ PW.GAS

**PW.GAS** is a modular Gameplay Ability System framework for **Unreal Engine 5**,  
developed by **Metehan Goksel Kurtulan (Iompaeqe)** as part of *ProjectW* —  
a multiplayer prototype built around a tag-driven, modular architecture.

This repository contains the three GAS-related core modules extracted and refactored from *ProjectW*.

---

## 🧩 Modules

| Module | Status | Description |
|:--|:--|:--|
| **PWGASCore** | ⚙️ *Finalizing* | Core framework extending Unreal’s Ability System with lifecycle management, ability tasks, async utilities, and custom effect context. |
| **PWAttributes** | ✅ *Stable* | Modular AttributeSet system for scalable stat management and initialization. |
| **PWGASIntegration** | 🧠 *Planned* | Integration bridge between GASCore and gameplay systems such as Inventory, Projectiles, and Shop. |

---

## ⚙️ Overview

**PW.GAS** expands Unreal’s default Gameplay Ability System with:

- Extended **AbilitySystemComponent (ASC)** featuring lifecycle, data, and event helpers.  
- Modular **Ability** base classes with tag-based activation and identification.  
- Custom **GameplayEffectContext** supporting critical hits, blocks, and knockback impulses.  
- Reusable **AbilityTasks** and **AsyncTasks** (e.g. cooldown tracking, target data collection).  
- Utility libraries for **tag operations**, **attribute math**, and **effect context manipulation**.  
- Clean C++/Blueprint balance — built for extensibility and clarity.

---

## 🧩 Usage Highlights

- Derive your gameplay abilities from `UPWGameplayAbilityBase`.  
- Use `UPWAbilitySystemComponent` for ability registration, lifecycle control, and tag management.  
- Track cooldowns and targeting using `UWaitCooldownChange`, `UWaitCooldownsChange`, or `UTargetDataUnderMouse`.  
- Extend or override `UPWGameplayEffectContext` for custom hit logic.  

---

## 🔗 Integration With Other PW Modules

**PW.GAS** is part of a larger modular ecosystem, including:

- **PW.ProjectileFramework** – Reusable projectile, hit, and VFX logic.  
- **PW.Inventory / PW.Shop** – Gameplay systems that integrate through `PWGASIntegration`.  
- **PW.SharedTypes / PW.Contracts** – Common data types and interfaces used across PW modules.

---

## 🧠 Background

PW.GAS was originally developed for *ProjectW*,  
a top-down arena prototype inspired by *Warlock* and *Spellsworn*.  
It has since evolved into a clean, standalone GAS framework  
intended for use across any Unreal Engine 5 project.

---

## 🧾 License

This project is licensed under the **MIT License**.  
See the [LICENSE](LICENSE) file for full details.

---

**Author:** Metehan Goksel Kurtulan
🎮 Game Developer – Systems & Gameplay Engineering
