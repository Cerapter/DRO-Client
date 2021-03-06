#ifndef COURTROOM_H
#define COURTROOM_H

#include "aobasshandle.h"
#include "aoblipplayer.h"
#include "aobutton.h"
#include "aocharbutton.h"
#include "aocharmovie.h"
#include "aoconfig.h"
#include "aoconfigpanel.h"
#include "aoemotebutton.h"
#include "aoevidencebutton.h"
#include "aoevidencedisplay.h"
#include "aoimage.h"
#include "aolabel.h"
#include "aolineedit.h"
#include "aomovie.h"
#include "aomusicplayer.h"
#include "aonotearea.h"
#include "aonotepad.h"
#include "aopacket.h"
#include "aoscene.h"
#include "aosfxplayer.h"
#include "aoshoutplayer.h"
#include "aotextarea.h"
#include "aoevidencedescription.h"
#include "aotimer.h"
#include "datatypes.h"

#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDateTime>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QMap>
#include <QPlainTextEdit>
#include <QPropertyAnimation>
#include <QRect>
#include <QSignalMapper>
#include <QSlider>
#include <QStack>
#include <QTextBrowser>
#include <QVector>

class AOApplication;

class Courtroom : public QMainWindow
{
    Q_OBJECT
public:
    explicit Courtroom(AOApplication *p_ao_app);

    void append_char(char_type p_char) { char_list.append(p_char); }
    void append_evidence(evi_type p_evi) { evidence_list.append(p_evi); }
    void append_music(QString f_music) { music_list.append(f_music); }
    void append_area(QString f_area) { area_list.append(f_area); }
    void clear_music() { music_list.clear(); }
    void clear_areas() { area_list.clear(); }

    void fix_last_area()
    {
        if (area_list.size() > 0)
        {
            QString malplaced = area_list.last();
            area_list.removeLast();
            append_music(malplaced);
            //      qDebug() << "what" << malplaced;
        }
    }

    //sets position of widgets based on theme ini files
    void set_widgets();
    //sets font properties based on theme ini files
    void set_font(QWidget *widget, QString p_identifier);
    //same as above, but use override color as color if it is not an empty string, otherwise use
    //normal logic for color of set_font
    void set_font(QWidget *widget, QString p_identifier, QString override_color);
    //sets font properties for QTextEdit (same as above but also text outline)
    void set_qtextedit_font(QTextEdit *widget, QString p_identifier);
    //same as second set_font but for qtextedit
    void set_qtextedit_font(QTextEdit *widget, QString p_identifier, QString override_color);
    //helper function that calls above function on the relevant widgets
    void set_fonts();

    //sets dropdown menu stylesheet
    void set_dropdown(QWidget *widget, QString target_tag);

    //helper funciton that call above function on the relevant widgets
    void set_dropdowns();

    void set_window_title(QString p_title);

    //reads theme inis and sets size and pos based on the identifier
    void set_size_and_pos(QWidget *p_widget, QString p_identifier);

    //sets status as taken on character with cid n_char and places proper shading on charselect
    void set_taken(int n_char, bool p_taken);

    //sets the current background to argument. also does some checks to see if it's a legacy bg
    void set_background(QString p_background);

    //sets the evidence list member variable to argument
    void set_evidence_list(QVector<evi_type> &p_evi_list);

    //sets the character position
    void set_character_position(QString p_pos, bool refresh_dropdown);

    //called when a DONE#% from the server was received
    void done_received();

    //sets the local mute list based on characters available on the server
    void set_mute_list();

    //sets desk and bg based on pos in chatmessage
    void set_scene();

    //sets text color based on text color in chatmessage
    void set_text_color();

    //takes in serverD-formatted IP list as prints a converted version to server OOC
    //admittedly poorly named
    void set_ip_list(QString p_list);

    //disables chat if current cid matches second argument
    //enables if p_muted is false
    void set_mute(bool p_muted, int p_cid);

    //send a message that the player is banned and quits the server
    void set_ban(int p_cid);

    //implementations in path_functions.cpp
    QString get_background_path();
    QString get_default_background_path();

    //cid = character id, returns the cid of the currently selected character
    int get_cid() { return m_cid; }
    QString get_current_char() { return current_char; }

    //properly sets up some varibles: resets user state
    void enter_courtroom(int p_cid);

    //helper function that populates ui_music_list with the contents of music_list
    void list_music();

    void list_areas();

    void list_sfx();

    void list_note_files();

    void set_note_files();

    void move_widget(QWidget *p_widget, QString p_identifier);

    void set_shouts();
    void set_effects();
    void set_judge_enabled(bool p_enabled);
    void set_judge_wtce();
    void set_free_blocks();

    //these are for OOC chat
    void append_server_chatmessage(QString p_name, QString p_message);

    //these functions handle chatmessages sequentially.
    //The process itself is very convoluted and merits separate documentation
    //But the general idea is objection animation->pre animation->talking->idle
    void handle_chatmessage(QStringList *p_contents);
    void handle_chatmessage_2();
    void handle_chatmessage_3();

    //adds text to the IC chatlog. p_name first as bold then p_text then a newlin
    //this function keeps the chatlog scrolled to the top unless there's text selected
    // or the user isn't already scrolled to the top
    void update_ic_log(bool p_reset_log);
    void append_ic_text(QString p_name, QString p_line, bool p_system, bool p_music);
    void append_system_text(QString p_line);

    //prints who played the song to IC chat and plays said song(if found on local filesystem)
    //takes in a list where the first element is the song name and the second is the char id of who played it
    void handle_song(QStringList *p_contents);

    //animates music text
    void handle_music_anim();

    //handle server-side clock animation and display
    void handle_clock(QString time);

    //handle request to change theme variant
    void handle_theme_variant(QString theme_variant);

    void play_preanim();

    //plays the witness testimony or cross examination animation based on argument
    void handle_wtce(QString p_wtce);

    //sets the hp bar of defense(p_bar 1) or pro(p_bar 2)
    //state is an number between 0 and 10 inclusive
    void set_hp_bar(int p_bar, int p_state);

    void check_connection_received();

    //checks whether shout/effect/wtce/free block files are found
    void check_shouts();
    void check_effects();
    void check_wtce();
    void check_free_blocks();

    void resume_timer(int timer_id);
    void set_timer_time(int timer_id, int new_time);
    void set_timer_timestep(int timer_id, int timestep_length);
    void set_timer_firing(int timer_id, int firing_interval);
    void pause_timer(int timer_id);

    template <typename T>
    int adapt_numbered_items(QVector<T *> &item_vector, QString config_item_number, QString item_name);

signals:
    void closing();

private:
    AOApplication *ao_app = nullptr;
    AOConfig *ao_config   = nullptr;

    int m_courtroom_width  = 714;
    int m_courtroom_height = 668;

    int m_viewport_x = 0;
    int m_viewport_y = 0;

    int m_viewport_width  = 256;
    int m_viewport_height = 192;

    QVector<char_type> char_list;
    QVector<evi_type> evidence_list;
    QVector<QString> music_list;
    QVector<QString> area_list;
    QVector<QString> sfx_names;
    QVector<QString> area_names;
    QVector<QString> note_list;

    QSignalMapper *char_button_mapper;

    //triggers ping_server() every 60 seconds
    QTimer *keepalive_timer;

    //maintains a timer for how fast messages tick onto screen
    QTimer *chat_tick_timer;
    //which tick position(character in chat message) we are at
    int tick_pos = 0;
    //used to determine how often blips sound
    int blip_pos             = 0;
    int rainbow_counter      = 0;
    bool rainbow_appended    = false;
    bool note_shown          = false;
    bool contains_add_button = false;

    //////////////
    QScrollArea *note_scroll_area;

    //delay before chat messages starts ticking
    QTimer *text_delay_timer;

    //delay before sfx plays
    QTimer *sfx_delay_timer;

    //keeps track of how long realization is visible(it's just a white square and should be visible less than a second)
    QTimer *realization_timer;

    //times how long the blinking testimony should be shown(green one in the corner)
    QTimer *testimony_show_timer;
    //times how long the blinking testimony should be hidden
    QTimer *testimony_hide_timer;

    //Generate a File Name based on the time you launched the client
    QString icchatlogsfilename = QDateTime::currentDateTime().toString("'logs/'ddd MMMM dd yyyy hh.mm.ss.z'.txt'");

    //configuration files locations
    QString rpc_ini         = "configs/rpccharlist.ini";
    QString file_select_ini = "configs/filesabstract.ini";
    QString shownames_ini   = "configs/shownames.ini";

    //theme files locations
    QString design_ini    = "courtroom_design.ini";
    QString fonts_ini     = "courtroom_fonts.ini";
    QString cc_config_ini = "courtroom_config.ini";
    QString cc_sounds_ini = "courtroom_sounds.ini";

    //every time point in char.inis times this equals the final time
    const int time_mod = 40;

    static const int chatmessage_size = 16;
    QString m_chatmessage[chatmessage_size];
    bool chatmessage_is_empty = false;

    QString previous_ic_message = "";

    QColor m_base_string_color;
    QString m_string_color = "";

    QStack<QString> m_color_stack;

    bool testimony_in_progress = false;

    //in milliseconds
    const int testimony_show_time = 1500;

    //in milliseconds
    const int testimony_hide_time = 500;

    //char id, muted or not
    QMap<int, bool> mute_map;

    //QVector<int> muted_cids;

    bool is_client_muted = false;

    // state of animation, 0 = objecting, 1 = preanim, 2 = talking, 3 = idle
    int anim_state = 3;

    // state of text ticking, 0 = not yet ticking, 1 = ticking in progress, 2 = ticking done
    int text_state = 2;

    // character id, which index of the char_list the player is
    int m_cid = -1;

    // if enabled, disable showing our own sprites when we talk in ic
    bool m_msg_is_first_person = false;

    //cid and this may differ in cases of ini-editing
    QString current_char = "";

    QString current_file = "";

    int m_objection_state       = 0;
    int m_effect_state          = 0;
    int m_text_color            = 0;
    int m_shout_state           = 0;
    int m_effect_current        = 0;
    int m_wtce_current          = 0;
    bool is_presenting_evidence = false;
    bool is_judge               = false;
    bool is_system_speaking     = false;

    int defense_bar_state     = 0;
    int prosecution_bar_state = 0;

    int current_char_page = 0;
    int char_columns      = 10;
    int char_rows         = 9;
    int max_chars_on_page = 90;

    int current_emote_page = 0;
    int current_emote      = 0;
    int prev_emote         = 0;
    int emote_columns      = 5;
    int emote_rows         = 2;
    int max_emotes_on_page = 10;

    int m_chatlog_limit       = 200;
    bool m_chatlog_newline    = false;
    bool m_chatlog_scrolldown = false;

    //  inmchatlog_changed;

    QVector<evi_type> local_evidence_list;

    int current_evidence_page = 0;
    int current_evidence      = 0;
    int evidence_columns      = 6;
    int evidence_rows         = 3;
    int max_evidence_on_page  = 18;

    int current_clock = -1;
    int timer_number  = 0;

    QString current_background = "gs4";

    AOImage *ui_background;

    QWidget *ui_viewport;
    AOScene *ui_vp_background;
    AOMovie *ui_vp_speedlines;
    AOCharMovie *ui_vp_player_char;
    AOScene *ui_vp_desk;
    AOEvidenceDisplay *ui_vp_evidence_display;

    AONoteArea *ui_note_area;

    //  AONotepad *ui_vp_notepad;
    // list of characters that require a second application ID
    // note that since it's hardcoded, it won't be of much use in other servers
    QVector<QString> rpc_char_list;

    AOImage *ui_vp_notepad_image;
    QTextEdit *ui_vp_notepad;

    AOImage *ui_vp_chatbox   = nullptr;
    QTextEdit *ui_vp_showname   = nullptr;
    QTextEdit *ui_vp_message = nullptr;
    AOImage *ui_vp_testimony = nullptr;
    AOMovie *ui_vp_effect    = nullptr;
    AOMovie *ui_vp_wtce      = nullptr;
    AOMovie *ui_vp_objection = nullptr;

    AOImage *ui_vp_music_display_a = nullptr;
    AOImage *ui_vp_music_display_b = nullptr;

    AOImage *ui_vp_showname_image = nullptr;

    QTextEdit *ui_vp_music_name    = nullptr;
    QPropertyAnimation *music_anim = nullptr;

    QWidget *ui_vp_music_area;

    AOMovie *ui_vp_clock;
    QVector<AOTimer *> ui_timers;

    QTextEdit *ui_ic_chatlog = nullptr;
    record_type_array m_ic_records;

    AOTextArea *ui_server_chatlog;

    QListWidget *ui_mute_list;
    QListWidget *ui_area_list;
    QListWidget *ui_music_list;
    QListWidget *ui_sfx_list;

    QLineEdit *ui_ic_chat_message;

    QLineEdit *ui_ooc_chat_message;
    QLineEdit *ui_ooc_chat_name;

    QLineEdit *ui_music_search;

    QLineEdit *ui_sfx_search;

    QWidget *ui_emotes = nullptr;
    QVector<AOEmoteButton *> ui_emote_list;
    AOButton *ui_emote_left;
    AOButton *ui_emote_right;

    QComboBox *ui_emote_dropdown;
    QComboBox *ui_pos_dropdown;

    AOImage *ui_defense_bar;
    AOImage *ui_prosecution_bar;

    //buttons to cycle through shouts
    AOButton *ui_shout_up   = nullptr;
    AOButton *ui_shout_down = nullptr;
    //buttons to cycle through effects
    AOButton *ui_effect_up   = nullptr;
    AOButton *ui_effect_down = nullptr;
    //buttons to cycle through wtce
    AOButton *ui_wtce_up   = nullptr;
    AOButton *ui_wtce_down = nullptr;

    //holds all the shout button objects
    QVector<AOButton *> ui_shouts;
    //holds all the effect button objects
    QVector<AOButton *> ui_effects;
    //holds all the shout buttons objects
    QVector<AOButton *> ui_wtce;
    //holds all the free block objects
    QVector<AOMovie *> ui_free_blocks;

    //holds all the names for sound files for the shouts
    //  QVector<QString> shout_names = {"holdit", "objection", "takethat", "custom", "gotit", "crossswords", "counteralt"};
    QVector<QString> shout_names;

    //holds all the names for sound/anim files for the effects
    //QVector<QString> effect_names = {"effect_flash", "effect_gloom", "effect_question", "effect_pow"};
    QVector<QString> effect_names;

    //holds all the names for sound/anim files for the shouts
    //QVector<QString> shout_names = {"witnesstestimony", "crossexamination", "investigation", "nonstop"};
    QVector<QString> wtce_names;

    //holds all the names for free blocks
    QVector<QString> free_block_names;

    //holds whether the animation file exists for a determined shout/effect
    QVector<bool> shouts_enabled;
    QVector<bool> effects_enabled;
    QVector<bool> wtce_enabled;
    QVector<bool> free_blocks_enabled;

    //  AOButton* ui_shout_hold_it      = nullptr; // 1
    //  AOButton* ui_shout_objection    = nullptr; // 2
    //  AOButton* ui_shout_take_that    = nullptr; // 3
    //  AOButton* ui_shout_custom       = nullptr; // 4
    //  AOButton* ui_shout_got_it       = nullptr; // 5
    //  AOButton* ui_shout_cross_swords = nullptr; // 6
    //  AOButton* ui_shout_counter_alt  = nullptr; // 7

    AOButton *ui_witness_testimony;
    AOButton *ui_cross_examination;
    AOButton *ui_investigation;
    AOButton *ui_nonstop;

    AOButton *ui_change_character;
    AOButton *ui_call_mod;
    AOButton *ui_switch_area_music;

    AOButton *ui_config_panel;

    AOButton *ui_set_notes;

    QCheckBox *ui_pre;
    QCheckBox *ui_flip;
    QCheckBox *ui_hidden;

    QVector<QCheckBox *> ui_checks; // 0 = pre, 1 = flip, 2 = hidden
    QVector<AOLabel *> ui_labels;   // 0 = music, 1 = sfx, 2 = blip
    QVector<AOImage *> ui_label_images;
    QVector<QString> label_images = {"Pre", "Flip", "Hidden", "Music", "SFX", "Blip"};

    AOButton *ui_effect_flash = nullptr;
    AOButton *ui_effect_gloom = nullptr;

    AOButton *ui_mute = nullptr;

    AOButton *ui_defense_plus;
    AOButton *ui_defense_minus;

    AOButton *ui_prosecution_plus;
    AOButton *ui_prosecution_minus;

    QComboBox *ui_text_color;

    AOImage *ui_muted;

    AOButton *ui_note_button;

    AOButton *ui_evidence_button;
    AOImage *ui_evidence;
    AOLineEdit *ui_evidence_name;
    QWidget *ui_evidence_buttons;
    QVector<AOEvidenceButton *> ui_evidence_list;
    AOButton *ui_evidence_left;
    AOButton *ui_evidence_right;
    AOButton *ui_evidence_present;
    AOImage *ui_evidence_overlay;
    AOButton *ui_evidence_delete;
    AOLineEdit *ui_evidence_image_name;
    AOButton *ui_evidence_image_button;
    AOButton *ui_evidence_x;
    AOEvidenceDescription *ui_evidence_description;

    AOImage *ui_char_select_background;

    //abstract widget to hold char buttons
    QWidget *ui_char_buttons         = nullptr;
    AOImage *ui_char_button_selector = nullptr;
    QVector<AOCharButton *> ui_char_button_list;

    AOButton *ui_back_to_lobby;

    AOButton *ui_char_select_left;
    AOButton *ui_char_select_right;

    AOButton *ui_spectator;

    QHash<QString, QWidget *> widget_names;

    void create_widgets();
    void connect_widgets();
    void set_widget_names();
    void reset_widget_names();
    void insert_widget_name(QString p_widget_name, QWidget *p_widget);
    void insert_widget_names(QVector<QString> &p_widget_names, QVector<QWidget *> &p_widgets);
    template <typename T>
    void insert_widget_names(QVector<QString> &p_widget_names, QVector<T *> &p_widgets);
    void set_widget_layers();

    void construct_char_select();
    void reconstruct_char_select();
    void reset_char_select();
    void set_char_select();
    void set_char_select_page();

    void construct_emotes();
    void reconstruct_emotes();
    void reset_emote_page();
    void set_emote_page();
    void set_emote_dropdown();

    void construct_evidence();
    void set_evidence_page();

    void load_note();
    void save_note();
    void save_textlog(QString p_text);

    void set_char_rpc();

public slots:
    void objection_done();
    void preanim_done();

    void realization_done();

    void show_testimony();
    void hide_testimony();

    void mod_called(QString p_ip);

private slots:
    void start_chat_ticking();
    void play_sfx();

    void chat_tick();

    void on_mute_list_clicked(QModelIndex p_index);

    void on_chat_return_pressed();
    void on_chat_config_changed();

    void on_ooc_return_pressed();

    void on_music_search_edited(QString p_text);
    void on_music_list_clicked();
    void on_area_list_clicked();
    void on_music_list_double_clicked(QModelIndex p_model);
    void on_area_list_double_clicked(QModelIndex p_model);

    void on_sfx_search_edited(QString p_text);

    void select_emote(int p_id);

    void on_emote_clicked(int p_id);

    void on_emote_left_clicked();
    void on_emote_right_clicked();

    void on_emote_dropdown_changed(int p_index);
    void on_pos_dropdown_changed(int p_index);

    void on_evidence_name_edited();
    void on_evidence_image_name_edited();
    void on_evidence_image_button_clicked();
    void on_evidence_clicked(int p_id);
    void on_evidence_double_clicked(int p_id);

    void on_evidence_hover(int p_id, bool p_state);

    void on_evidence_left_clicked();
    void on_evidence_right_clicked();
    void on_evidence_present_clicked();

    void on_cycle_clicked();

    void cycle_shout(int p_index);
    void cycle_effect(int p_index);
    void cycle_wtce(int p_index);

    void on_add_button_clicked();
    void on_delete_button_clicked();

    void on_set_file_button_clicked();
    void on_file_selected();

    void delete_widget(QWidget *p_widget);
    void load_shouts();
    void load_effects();
    void load_wtce();
    void load_free_blocks();
    /**
   * @brief reset the shout button's texture to default
   * DOES NOT MODIFY OBJECTION_STATE
   */
    void reset_shout_buttons();

    /**
   * @brief a general purpose function to toggle button selection
   */
    void on_shout_clicked();

    void reset_effect_buttons();
    void on_effect_button_clicked();

    void on_mute_clicked();

    void on_defense_minus_clicked();
    void on_defense_plus_clicked();
    void on_prosecution_minus_clicked();
    void on_prosecution_plus_clicked();

    void on_text_color_changed(int p_color);

    void on_witness_testimony_clicked();
    void on_cross_examination_clicked();
    void reset_judge_wtce_buttons();
    void on_wtce_clicked();

    void on_change_character_clicked();
    void on_app_reload_theme_requested();
    void on_call_mod_clicked();

    void on_switch_area_music_clicked();

    void on_config_panel_clicked();
    void on_note_button_clicked();

    void on_set_notes_clicked();

    void on_note_text_changed();

    void on_pre_clicked();
    void on_flip_clicked();
    void on_hidden_clicked();

    void on_sfx_list_clicked();

    void on_evidence_button_clicked();

    void on_evidence_delete_clicked();
    void on_evidence_x_clicked();

    void on_back_to_lobby_clicked();

    void on_char_select_left_clicked();
    void on_char_select_right_clicked();
    void char_clicked(int n_char);
    void char_mouse_entered(AOCharButton *p_caller);
    void char_mouse_left();

    void on_spectator_clicked();

    void ping_server();

/*!
 * =============================================================================
 * AUDIO SYSTEM
 */

public:
    bool is_audio_muted();

public slots:
    void set_audio_mute_enabled(bool p_enabled);
    void set_effects_volume(int p_volume);
    void set_system_volume(int p_volume);
    void set_music_volume(int p_volume);
    void set_blips_volume(int p_volume);

private:
    bool m_audio_mute              = false;
    AOSfxPlayer *m_effects_player  = nullptr;
    AOShoutPlayer *m_shouts_player = nullptr;
    AOSfxPlayer *m_system_player   = nullptr;
    AOMusicPlayer *m_music_player  = nullptr;
    AOBlipPlayer *m_blips_player   = nullptr;

private slots:
    void on_config_effects_volume_changed(int p_volume);
    void on_config_system_volume_changed(int p_volume);
    void on_config_music_volume_changed(int p_volume);
    void on_config_blips_volume_changed(int p_volume);

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

template <typename T>
void Courtroom::insert_widget_names(QVector<QString> &p_widget_names, QVector<T *> &p_widgets)
{
    QVector<QWidget *> widgets;

    for (QWidget *widget : p_widgets)
        widgets.append(widget);

    insert_widget_names(p_widget_names, widgets);
}

#endif // COURTROOM_H
