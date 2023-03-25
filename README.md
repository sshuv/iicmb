# IICMB

## Description

The IICMB core provides low-speed, two-wire, bidirectional serial bus interfaces compliant to industry standard I<sup>2</sup>C protocol.
The key feature of the core is its ability to control several connected I<sup>2</sup>C buses effectively reducing complexity of the system.

## Features

- Compatible with Philips' I<sup>2</sup>C standard
- Works with up to 16 distinct I<sup>2</sup>C buses
- Statically configurable system bus clock frequency
- Statically configurable desired clock frequencies of I<sup>2</sup>C buses
- Multi-master clock synchronization
- Multi-master arbitration
- Clock stretching
- Digital filtering of SCL and SDA inputs
- Standard (up to 100 kHz) and Fast (up to 400 kHz) mode operation
- Example connection as 8-bit slave on Wishbone bus
- Example connection as 32-bit slave on Avalon-MM bus
- Sequencer-based example, working without any system bus
- Low-level C driver [poll](./software/poll/iicmb.h) and [irq](./software/irq/README.md) based
