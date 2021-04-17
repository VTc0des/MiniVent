#include "panel.h"

Panel::Panel() {}

void Panel::updateTime() {
  // Update and display time if a second has passed.
  if (!(millis() % 1000)) {
    // Increase time.
    _vs.seconds++;         // increase seconds
    if (_vs.seconds == 60) // If it's been a minute
    {
      _vs.seconds = 0;      // start over seconds
      _vs.minute++;         // Increase minutes
      if (_vs.minute == 60) // If it's been an hour
      {
        _vs.minute = 0; // start over minutes
        _vs.hours++;    // increase hours

        // Assuming device will not be run for more than 99 hours.
      }
    }
  }
}

SplashPanel::SplashPanel(String *text, int display_time, Panel **next_ptr)
    : _display_time(display_time), _text(text), _next_d_ptr(next_ptr) {}

void SplashPanel::start() {

  // Dereference double pointer to panel.
  _next_ptr = *_next_d_ptr;

  // Clear display.
  _vio.disp.clearDisplay();

  // Display each line.
  for (int i = 0; i < 4; i++) {
    _vio.disp.setCursor(0, i);
    _vio.disp.print(*(_text + i));
  }
}

Panel *SplashPanel::update() {

  // Wait for time and then pass next panel.
  delay(_display_time);
  return _next_ptr;
}

EditPanel::EditPanel(String top_text, Panel **run_panel_ptr,
                     Panel **stop_panel_ptr)
    : Panel(), _top_text(top_text), _run_panel_d_ptr(run_panel_ptr),
      _stop_panel_d_ptr(stop_panel_ptr) {
  // Build new encoder manager with 4 selections.
  _em_ptr = new EncoderManager(Panel::_vio.enc, 4);
}

void EditPanel::start() {

  // Dereference double pointer to panels.
  _run_panel_ptr = *_run_panel_d_ptr;
  if (_stop_panel_d_ptr != 0) {
    _stop_panel_ptr = *_stop_panel_d_ptr;
  } else {
    _stop_panel_ptr = 0;
  }

  // Start the selection encoder.
  _em_ptr->start();

  // Clear display.
  _vio.disp.clearDisplay();

  // Write cursor.
  _vio.disp.setCursor(0, 0);
  _vio.disp.print(">");

  // Write first line.
  _vio.disp.setCursor(1, 0);
  _vio.disp.print(_top_text);

  // Write second line and add default tidal volume value.
  _vio.disp.setCursor(1, 1);
  _vio.disp.print(_tv_text + _vs.tidal_volume + _tv_units);

  // Write third line and add default respiration rate value.
  _vio.disp.setCursor(1, 2);
  _vio.disp.print(_rr_text + NhdDisplay::zeroPad(_vs.respiration_rate) +
                  _rr_units);

  // Write fourth line and add default i:e ratio.
  _vio.disp.setCursor(1, 3);
  _vio.disp.print(_i_e_text + _vs.inhale + ':' + _vs.exhale);

  // Mark that the user hasn't made a change.
  _made_change = false;
}

Panel *EditPanel::update() {

  // Update encoder manager for new positions.
  _em_ptr->poll();

  // Check if we have a non-zero stop panel pointer and return if button pushed.
  if (_stop_panel_ptr != 0 && _vio.stop_button.getButtonState()) {
    _em_ptr->close();
    return _stop_panel_ptr;
  }

  // If we are not in edit mode and the button pushed, edit current selection.
  if (!_edit && _vio.enc_button.getButtonState()) {

    // Selected confirm settings, exit and run device.
    if (_row == 0) {

      if (_made_change) {

        // Reset time when endering edit panel.
        _vs.hours = 0;
        _vs.minute = 0;
        _vs.seconds = 0;
      }

      // Close the encoder manager and start runnning the device.
      _em_ptr->close();
      return _run_panel_ptr;
    }

    // Enable edit mode.
    _edit = true;

    // Set cursor to flash.
    _vio.disp.blinkingOn();

    // Set encoder manager to give selections based on selected quantity.
    int num_selections = 0;
    int starting_selection = 0;

    switch (_row) {
    // Set selections for tidal volume.
    case 1:
      num_selections = (_vl.max_tidal_volume - _vl.min_tidal_volume) /
                           _vl.delta_tidal_volume +
                       1;
      starting_selection =
          (_vs.tidal_volume - _vl.min_tidal_volume) / _vl.delta_tidal_volume;
      break;
    // Set selections for respiration rate.
    case 2:
      num_selections = (_vl.max_respiration_rate - _vl.min_respiration_rate) /
                           _vl.delta_respiration_rate +
                       1;
      starting_selection = (_vs.respiration_rate - _vl.min_respiration_rate) /
                           _vl.delta_respiration_rate;
      break;
    // Set selections for i:e ratio.
    case 3:
      num_selections = (_vl.max_exhale - _vl.min_exhale) / _vl.delta_exhale + 1;
      starting_selection = (_vs.exhale - _vl.min_exhale) / _vl.delta_exhale;
      break;
    }

    // Update settings to encoder manager.
    _em_ptr->setNumOptions(num_selections);
    _em_ptr->setSelection(starting_selection);
    _old_selection = starting_selection;

    // Move cursor to over the arrow for the row.
    _vio.disp.setCursor(0, _row);

    // If we are in edit mode and the button was not pushed, encoder movement
    // changes value.
  } else if (_edit && !_vio.enc_button.getButtonState()) {

    if (_em_ptr->getSelection() != _old_selection) {

      // Mark that the user made a change.
      _made_change = true;

      switch (_row) {
      // Edit tidal volume.
      case 1:

        // Calculate new tidal volume amount.
        _vs.tidal_volume = _vl.min_tidal_volume +
                           _em_ptr->getSelection() * _vl.delta_tidal_volume;

        // Write to the display.
        _vio.disp.setCursor(1 + _tv_text_length, 1);
        _vio.disp.print(_vs.tidal_volume);
        break;

      // Edit respiration rate to new value.
      case 2:

        // Calculate new respitory rate.
        _vs.respiration_rate =
            _vl.min_respiration_rate +
            _em_ptr->getSelection() * _vl.delta_respiration_rate;

        // Write to the display.
        _vio.disp.setCursor(1 + _rr_text_length, 2);
        _vio.disp.print(_vs.respiration_rate);
        break;

      // Edit i:e ratio.
      case 3:

        // Calculate new i:e ratio.
        _vs.exhale =
            _vl.min_exhale + _em_ptr->getSelection() * _vl.delta_exhale;

        // Write to the display.
        _vio.disp.setCursor(1 + _i_e_text_length, 3);
        _vio.disp.print(_vs.exhale);
        break;
      }

      // Set cursor back to row so blinking continues.
      _vio.disp.setCursor(0, _row);

      // Set old selection to the selection from this cycle.
      _old_selection = _em_ptr->getSelection();
    }

    // If we are in edit mode and the button was pushed, exit edit mode.
  } else if (_edit && _vio.enc_button.getButtonState()) {

    // Disable edit mode.
    _edit = false;

    // Stop cursor blinking.
    _vio.disp.blinkingOff();

    // Set encoder manager back to 4 selections and to the row.
    _em_ptr->setNumOptions(4);
    _em_ptr->setSelection(_row);

    // If we are not in edit mode and the button was not pushed, move cursoe.
  } else if (!_edit && !_vio.enc_button.getButtonState()) {

    // Check if we've moved the cursor.
    if (_em_ptr->getSelection() != _row) {

      // Remove old cursor on the display.
      _vio.disp.setCursor(0, _row);
      _vio.disp.remove();

      // Write the new cursor on the display.
      _vio.disp.setCursor(0, _em_ptr->getSelection());
      _vio.disp.print(">");

      // Set the old row to the current row.
      _row = _em_ptr->getSelection();
    }
  }

  return 0;
}

RunningPanel::RunningPanel(Panel **apply_panel_ptr, Panel **stop_panel_ptr)
    : _apply_panel_d_ptr(apply_panel_ptr), _stop_panel_d_ptr(stop_panel_ptr) {}

String RunningPanel::formatTime() {
  return NhdDisplay::zeroPad(_vs.hours) + ":" +
         NhdDisplay::zeroPad(_vs.minute) + ":" +
         NhdDisplay::zeroPad(_vs.seconds);
}

void RunningPanel::start() {

  // Dereference double pointer to panels.
  _apply_panel_ptr = *_apply_panel_d_ptr;
  _stop_panel_ptr = *_stop_panel_d_ptr;

  // Change mode to load new settings.
  _vs.mode = 'L';
  _vs.send = true;

  // Clear display.
  _vio.disp.clearDisplay();

  // Write first line.
  _vio.disp.setCursor(1, 0);
  _vio.disp.print(_top_text + formatTime());

  // Write second line and add default tidal volume value.
  _vio.disp.setCursor(1, 1);
  _vio.disp.print(_tv_text + _vs.tidal_volume + _tv_units);

  // Write third line and add default respiration rate value.
  _vio.disp.setCursor(1, 2);
  _vio.disp.print(_rr_text + NhdDisplay::zeroPad(_vs.respiration_rate) +
                  _rr_units);

  // Write fourth line and add default i:e ratio.
  _vio.disp.setCursor(1, 3);
  _vio.disp.print(_i_e_text + _vs.inhale + ':' + _vs.exhale);
}

Panel *RunningPanel::update() {

  // Check if stop button pushed and return stop panel if pushed.
  if (_vio.stop_button.getButtonState()) {
    return _stop_panel_ptr;
  }

  // Check if encoder button pushed and return apply panel if pushed.
  if (_vio.enc_button.getButtonState()) {
    return _apply_panel_ptr;
  }

  //Check if ihold button pushed and return ihold panel if pushed?

  updateTime();

  // Update and display time if a second has passed.
  if (!(millis() % 1000)) {
    // Update time on display.
    // Add 1 to the text length to accoud for empty column for cursor.
    _vio.disp.setCursor(_text_length_to_time + 1, 0);
    _vio.disp.print(formatTime());
  }
  return 0;
}

PausePanel::PausePanel(Panel **apply_panel_ptr, Panel **run_panel_ptr)
    : _apply_panel_d_ptr(apply_panel_ptr), _run_panel_d_ptr(run_panel_ptr) {
  // Build new encoder manager with 2 selections.
  _em_ptr = new EncoderManager(Panel::_vio.enc, 2);
}

void PausePanel::start() {

  // Dereference double pointer to panels.
  _apply_panel_ptr = *_apply_panel_d_ptr;
  _run_panel_ptr = *_run_panel_d_ptr;

  // Start a new encoder manager.
  _em_ptr->start();
  _em_ptr->setSelection(0);

  // Change mode to stop operation.
  _vs.mode = 'X';
  _vs.send = true;

  // Reset the selection.
  _selection = 0;

  // Clear display.
  _vio.disp.clearDisplay();

  // Write first line.
  _vio.disp.setCursor(1, 0);
  _vio.disp.print(_top_before_time);
  _vio.disp.print(NhdDisplay::zeroPad(_vs.hours) + ":" +
                  NhdDisplay::zeroPad(_vs.minute) + ":" +
                  NhdDisplay::zeroPad(_vs.seconds));
  _vio.disp.print(_top_after_time);

  // Write second line and add default tidal volume value.
  _vio.disp.setCursor(1, 1);
  _vio.disp.print(_tv_text + _vs.tidal_volume + _tv_units);

  // Write third line and add default respiration rate value.
  _vio.disp.setCursor(1, 2);
  _vio.disp.print(_rr_text + _vs.respiration_rate + _rr_units);

  // Write fourth line and add default i:e ratio.
  _vio.disp.setCursor(1, 3);
  _vio.disp.print(_i_e_text + _vs.inhale + ':' + _vs.exhale);

  // Set cursor over R in Run.
  _vio.disp.setCursor(_text_length_to_run, 0);
  _vio.disp.blinkingOn();
}

Panel *PausePanel::update() {

  // Poll encoder for updates.
  _em_ptr->poll();

  // If encoder button pushed over run, exit and start running.
  if (_selection == 0 && _vio.enc_button.getButtonState()) {
    _vio.disp.blinkingOff();
    _em_ptr->close();
    return _run_panel_ptr;
  } else if (_selection == 1 && _vio.enc_button.getButtonState()) {
    _vio.disp.blinkingOff();
    _em_ptr->close();
    return _apply_panel_ptr;
  }

  if (_em_ptr->getSelection() != _selection) {

    switch (_em_ptr->getSelection()) {
    // Cursor moved to run.
    case 0:
      _vio.disp.setCursor(_text_length_to_run, 0);
      break;
    // Cursor moved to edit.
    case 1:
      _vio.disp.setCursor(_text_length_to_edit, 0);
      break;
    }

    // Set the current selection to the old one.
    _selection = _em_ptr->getSelection();
  }
  return 0;
}