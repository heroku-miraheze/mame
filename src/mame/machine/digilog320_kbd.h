// license: BSD-3-Clause
// copyright-holders: Dirk Best
/***************************************************************************

    Digilog 320 Keyboard (HLE)

***************************************************************************/

#ifndef MAME_MACHINE_DIGILOG320_KBD_H
#define MAME_MACHINE_DIGILOG320_KBD_H

#pragma once

#include "machine/keyboard.h"
#include "diserial.h"


//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> digilog320_kbd_hle_device

class digilog320_kbd_hle_device : public device_t,
										public device_buffered_serial_interface<16>,
										protected device_matrix_keyboard_interface<5>
{
public:
	// construction/destruction
	digilog320_kbd_hle_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);

	// callbacks
	auto tx_handler() { return m_tx_handler.bind(); }

	// from host
	void rx_w(int state);

protected:
	// device_t overrides
	virtual ioport_constructor device_input_ports() const override;
	virtual void device_start() override;
	virtual void device_reset() override;

	// device_buffered_serial_interface overrides
	virtual void tra_callback() override;
	virtual void received_byte(uint8_t byte) override;

	// device_matrix_keyboard_interface overrides
	virtual void key_make(uint8_t row, uint8_t column) override;
	virtual void key_break(uint8_t row, uint8_t column) override;
	virtual void key_repeat(uint8_t row, uint8_t column) override;

private:
	required_ioport m_modifiers;

	uint8_t translate(uint8_t row, uint8_t column);

	devcb_write_line m_tx_handler;
};

// device type definition
DECLARE_DEVICE_TYPE(DIGILOG320_KBD_HLE, digilog320_kbd_hle_device)

#endif // MAME_MACHINE_DIGILOG320_KBD_H
