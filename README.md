# Scarlet Stamina System
A simple UE5 stamina system, originally developed for Zaris project.

**Status: Ready**

---
### General

***Stamina*** - is a movement / combat *resource*. It limits the number of certain moves the player is able to make in a short amount of time. When stamina is not being consumed, it is recovered. The amount of stamina is limited by *Max Stamina* value.

Every time any amount of stamina is consumed a *Recovery* timer is started, once it is over, stamina recovery occurs. The amount of stamina recovered every second during the recovery is determined by *Stamina Recovery Rate*.

When running out of stamina completely, a different - longer *Stamina Depletion Timeout* is used. Additionally, no stamina use can occur after depletion, until stamina values recovers above *Depletion Recovery Threshold*.

User can:
* Check if there is enough stamina with `IsEnoughStamina(float) -> bool`;
* Stamina is spent using the `ConsumeStamina(float) -> bool` method;
* Force recover stamina using `RecoverStamina(float) -> void` method.

---
### Implementation

![](Documentation/Code%20Design.png)
