#ifndef panel_h
#define panel_h

#include "Encoder.h"
#include "alarm.h"
#include "buttonmanager.h"
#include "encodermanager.h"
#include "nhd_0420d3z.h"
#include "ventproperties.h"

class Panel {
public:
  Panel();

  // Virtual methods for each panel to inherit.
  virtual void start() = 0;
  virtual Panel *update() = 0;

  void updateTime();

protected:
  // Pointers for system setting.
  static VentIO &_vio;
  static VentSettings &_vs;
  static VentLimits &_vl;
};

class SplashPanel : public Panel {
public:
  SplashPanel(String *text, int display_time, Panel **next_ptr);

  // Panel to display splash text for a specified amout of time.
  void start() override;
  Panel *update() override;

private:
  // Number of miliseconds to display text for.
  int _display_time;

  // Pointer to 4 element string array with text to display.
  String *_text;

  // Pointer and double pointer to next panel.
  Panel **_next_d_ptr;
  Panel *_next_ptr;
};

class EditPanel : public Panel {
public:
  EditPanel(String top_text, Panel **run_panel_ptr, Panel **stop_panel_ptr);

  // Panel to edit ventilator parameters.
  void start() override;
  Panel *update() override;

private:
  // Pointer to encoder manager for this edit panel.
  EncoderManager *_em_ptr;

  // String for the top line of text.
  String _top_text;

  // Text to show for tidal volume.
  String _tv_text = "TV = ";
  int _tv_text_length = 5;
  String _tv_units = " mL";

  // Text to show for respiratory rate.
  String _rr_text = "RR = ";
  int _rr_text_length = 5;
  String _rr_units = " BPM";

  // Text to show for I:E ratio.
  String _i_e_text = "I:E = ";
  int _i_e_text_length = 8;

  // Pointers to other panels.
  Panel **_run_panel_d_ptr;
  Panel **_stop_panel_d_ptr;
  Panel *_run_panel_ptr;
  Panel *_stop_panel_ptr;

  // Holds which row is currently being selected.
  int _row = 0;

  // Holds if we are in edit mode or not.
  bool _edit = false;

  // Old selection to detect changes made.
  int _old_selection = 0;

  // Bool to hold if changes were made.
  bool _made_change = false;
};

class RunningPanel : public Panel {
public:
  RunningPanel(Panel **apply_panel_ptr, Panel **stop_panel_ptr);

  // Panel to display current time counter and current settings.
  void start() override;
  Panel *update() override;

  String formatTime();

private:
  // Pointers to other panels.
  Panel **_apply_panel_d_ptr;
  Panel **_stop_panel_d_ptr;
  Panel *_apply_panel_ptr;
  Panel *_stop_panel_ptr;

  // String for the top line of text.
  String _top_text = "Run Time: ";
  int _text_length_to_time = 10;

  // Text to show for tidal volume.
  String _tv_text = "TV = ";
  String _tv_units = " mL";

  // Text to show for respiratory rate.
  String _rr_text = "RR = ";
  String _rr_units = " BPM";

  // Text to show for I:E ratio.
  String _i_e_text = "I:E = ";
};

class PausePanel : public Panel {
public:
  PausePanel(Panel **apply_panel_ptr, Panel **run_panel_ptr);

  // Panel to display time ventilated and settings during pause.
  void start() override;
  Panel *update() override;

private:
  // Pointer to encoder manager for this edit panel.
  EncoderManager *_em_ptr;

  // Pointers to other panels.
  Panel **_apply_panel_d_ptr;
  Panel **_run_panel_d_ptr;
  Panel *_apply_panel_ptr;
  Panel *_run_panel_ptr;

  // String for top line with time and selection.
  String _top_before_time = "T";
  String _top_after_time = " Run/Edit?";
  int _text_length_to_run = 11;
  int _text_length_to_edit = 15;

  // Text to show for tidal volume.
  String _tv_text = "TV = ";
  String _tv_units = " mL";

  // Text to show for respiratory rate.
  String _rr_text = "RR = ";
  String _rr_units = " BPM";

  // Text to show for I:E ratio.
  String _i_e_text = "I:E = ";

  // Current selection of whether to run or edit.
  int _selection = 0;
};

class AlarmPanel : public Panel {

public:
  // Panel to display time ventilated and settings during pause.
  void start() override;
  Panel *update() override;

private:
  const Alarm &alarm;
};
#endif
