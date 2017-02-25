/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : configuration dialog
*******************************************************************************/
#include "../../globals.h"
#include <string>
#include <stdexcept>
#include <functional>
#include <memory>
#include "../../lang/config_lang.h"
#include "../../res/resource.h"
#include "../../events/utils/logger.h"
#include "controls/common.h"
#include "controls/dialog.h"
#include "controls/file_dialog.h"
#include "controls/tab_control.h"
#include "controls/combo_box.h"
#include "controls/label.hpp"
#include "controls/button.hpp"
#include "controls/msg_box.h"
#include "general_page.h"
#include "manager_page.h"
#include "profile_page.h"
#include "config_dialog.h"
using namespace config::dialog;
using namespace config::dialog::controls;
using namespace std::literals::string_literals;


/// @brief Create dialog box
/// @param[in] instance  Current instance handle
/// @throws runtime_error     Dialog creation error
/// @throws invalid_argument  Invalid instance
ConfigDialog::ConfigDialog(library_instance_t instance) : Dialog(instance)
{
    // load config values
    config::Config::init();
    try
    {
        getLanguageResource().setLanguage(config::Config::langCode, config::Config::langFilePath);
    }
    catch (...)
    {
        config::Config::langCode = lang::langcode_t::english;
    }

    // set tabs and pages
    tab_association_t tabGeneral, tabManager, tabProfile;
    tabGeneral.button = std::make_shared<TabButton>(instance, getLanguageResource().dialog.generalSettings, IDC_GENERAL_TAB, IDB_CONFIG_ICONS, 1, 48);
    tabGeneral.page = std::make_shared<GeneralPage>(instance, (Dialog*)this);
    tabManager.button = std::make_shared<TabButton>(instance, getLanguageResource().dialog.profileManagement, IDC_MANAGER_TAB, IDB_CONFIG_ICONS, 2, 48);
    tabManager.page = std::make_shared<ManagerPage>(instance, (Dialog*)this);
    tabProfile.button = std::make_shared<TabButton>(instance, getLanguageResource().dialog.profileSettings, IDC_PROFILE_TAB, IDB_CONFIG_ICONS, 3, 48);
    tabProfile.page = std::make_shared<ProfilePage>(instance, (Dialog*)this);
    m_pProfilePage = (ProfilePage*)tabProfile.page.get();
    // insert pages
    getTabControl().addTab(tabGeneral, IDC_GENERAL_TAB);
    getTabControl().addTab(tabManager, IDC_MANAGER_TAB);
    getTabControl().addTab(tabProfile, IDC_PROFILE_TAB);

    // set event handlers
    Dialog::event_handler_t eventHandler;
    eventHandler.handler = onInit;
    Dialog::registerEvent(dialog_event_t::init, eventHandler);
    eventHandler.handler = onPaint;
    Dialog::registerEvent(dialog_event_t::paint, eventHandler);
    eventHandler.handler = onCommand;
    Dialog::registerEvent(dialog_event_t::command, eventHandler);
    eventHandler.handler = onMouseWheel;
    Dialog::registerEvent(dialog_event_t::wheelY, eventHandler);
    eventHandler.handler = onConfirm;
    Dialog::registerEvent(dialog_event_t::confirm, eventHandler);
    eventHandler.handler = onClose;
    Dialog::registerEvent(dialog_event_t::close, eventHandler);
}

/// @brief Destroy dialog box
ConfigDialog::~ConfigDialog()
{
    getTabControl().clear();
    config::Config::close();
}

/// @brief Display modal dialog box
/// @returns Dialog result
/// @throws runtime_error  Dialog creation/display error
Dialog::result_t ConfigDialog::showDialog()
{
    return Dialog::showDialog(IDD_CONFIG_DIALOG, reinterpret_cast<window_handle_t>(DIALOG_USE_BASE_WINDOW), L""s, true);
}



// -- event handlers -- --------------------------------------------

/// @brief Initialization event handler
EVENT_RETURN ConfigDialog::onInit(Dialog::event_args_t args)
{
    ConfigDialog& parent = args.getParent<ConfigDialog>();

    // set list of profiles
    ComboBox::initValues(args.window, IDC_PROFILE_LIST, Config::getAllProfileNames(), 0u);
    Label::setVisible(args.window, IDC_PROFILE_LIST, false); // hide (only visible when profile tab is active)
    Label::setVisible(args.window, IDS_PROFILE, false);
    // translate window components
    parent.onLanguageChange(args, false);

    // set language selector
    std::vector<std::wstring> langNames;
    uint32_t langNumber = static_cast<uint32_t>(LANGCODE_LAST_INTERNAL) + 1u; // built-in languages + external
    for (uint32_t i = 0u; i <= langNumber; ++i)
        langNames.push_back(lang::langcodeNames[i]);
    ComboBox::initValues(args.window, IDC_LANG_LIST, langNames, static_cast<int32_t>(Config::langCode));

    // add tab control and pages to dialog
    if (parent.getTabControl().create(args.window, LOGO_HEIGHT, LOGO_WIDTH) == false)
    {
        MsgBox::showMessage(L"Initialization error"s, L"Failed to load page content..."s, args.window,
                            MsgBox::button_set_t::ok, MsgBox::message_icon_t::error);
        return EVENT_RETURN_ERROR;
    }
    return EVENT_RETURN_VALID;
}


/// @brief Paint event handler - draw dialog
EVENT_RETURN ConfigDialog::onPaint(Dialog::event_args_t args)
{
    ConfigDialog& parent = args.getParent<ConfigDialog>();
    return parent.getTabControl().onPaint(args);
}


/// @brief Sub-control command event handler
EVENT_RETURN ConfigDialog::onCommand(Dialog::event_args_t args)
{
    ConfigDialog& parent = args.getParent<ConfigDialog>();

    // language selection event
    if (args.controlId() == IDC_LANG_LIST && args.isEventAction(ComboBox::event_t::selectionChanged))
    {
        int32_t buffer;
        if (ComboBox::getSelectedIndex(args.window, IDC_LANG_LIST, buffer))
        {
            if (parent.onLanguageSelection(args, buffer))
            {
                parent.onLanguageChange(args, true); // translate window components
            }
        }
        else
        {
            MsgBox::showMessage(L"An error occurred during selection. Please try again."s, L"Language selection error"s, 
                                args.window, MsgBox::button_set_t::ok, MsgBox::message_icon_t::error);
            return EVENT_RETURN_ERROR;
        }
    }
    // tab change
    else if (parent.getTabControl().onCommand(args) == EVENT_RETURN_VALID)
    {
        bool isProfilePage = parent.getTabControl().isActiveTab(IDC_PROFILE_TAB);
        Label::setVisible(args.window, IDC_PROFILE_LIST, isProfilePage);
        Label::setVisible(args.window, IDS_PROFILE, isProfilePage);
    }
    return EVENT_RETURN_IGNORE;
}


/// @brief Vertical mouse wheel event handler
EVENT_RETURN ConfigDialog::onMouseWheel(Dialog::event_args_t args)
{
    ConfigDialog& parent = args.getParent<ConfigDialog>();
    if (parent.getTabControl().isActiveTab(IDC_PROFILE_TAB))
    {
        parent.m_pProfilePage->onMouseWheel(args);
    }
    return EVENT_RETURN_IGNORE;
}


/// @brief Dialog confirm event handler - check validity
EVENT_RETURN ConfigDialog::onConfirm(Dialog::event_args_t args)
{
    ConfigDialog& parent = args.getParent<ConfigDialog>();

    // read and check values
    if (parent.getTabControl().onDialogConfirm(args) == false)
        return EVENT_RETURN_ERROR;

    // save config and profiles
    //...save values
    //...

    return EVENT_RETURN_VALID;
}


/// @brief Dialog close event handler
EVENT_RETURN ConfigDialog::onClose(Dialog::event_args_t args)
{
    ConfigDialog& parent = args.getParent<ConfigDialog>();
    parent.getTabControl().close();
    return EVENT_RETURN_VALID;
}



// -- specialized handlers -- --------------------------------------

/// @brief Language selection event
/// @param[in] value  Selected value
/// @returns Language validity / file validity
bool ConfigDialog::onLanguageSelection(Dialog::event_args_t& args, const int32_t value)
{
    // select custom language file
    if (value > static_cast<int32_t>(LANGCODE_LAST_INTERNAL))
    {
        try
        {
            // open file path choice dialog
            FileDialog fileDialog(reinterpret_cast<config::dialog::controls::library_instance_t>(m_instance), FileDialog::file_mode_t::load);
            if (fileDialog.showDialog(IDD_FILE_SELECT, IDC_FILE_PATH_EDIT, args.window, IDC_FILE_BTN_PATH, 
                                      Config::langFilePath, L"Select a language file..."s) == Dialog::result_t::confirm) // file choice success
            {
                Config::langCode = lang::langcode_t::customFile;
                Config::langFilePath = fileDialog.getFilePath();
            }

            // if cancelled or error, restore previous language selection
            if (Config::langCode != lang::langcode_t::customFile)
            {
                ComboBox::setSelectedIndex(args.window, IDC_LANG_LIST, static_cast<int32_t>(Config::langCode));
                return true;
            }
        }
        catch (const std::exception& exc)
        {
            events::utils::Logger::getInstance()->writeErrorEntry("ConfigDialog.onLanguageSelection"s, exc.what());
            return false;
        }
    }
    else // built-in language
    {
        Config::langCode = static_cast<lang::langcode_t>(value);
    }

    try
    {
        // update language resource
        getLanguageResource().setLanguage(Config::langCode, Config::langFilePath);
    }
    catch (...) // corrupted file
    {
        Config::langCode = lang::langcode_t::english;
        ComboBox::setSelectedIndex(args.window, IDC_LANG_LIST, static_cast<int32_t>(Config::langCode));
        MsgBox::showMessage(L"Invalid language file"s, L"Invalid / corrupted language file"s,
                            args.window, MsgBox::button_set_t::ok, MsgBox::message_icon_t::error);
        return false;
    }
    return true;
}


/// @brief Language change event
/// @param[in] isRecursive  Also translate controls in child pages or not
void ConfigDialog::onLanguageChange(Dialog::event_args_t& args, const bool isRecursive)
{
    // translate dialog controls
    Label::setText(args.window, IDS_PROFILE, getLanguageResource().profile.profileList);
    Button::setText(args.window, IDOK, getLanguageResource().dialog.confirm);
    Button::setText(args.window, IDCANCEL, getLanguageResource().dialog.cancel);

    // translate controls in embedded pages
    if (isRecursive)
    {
        getTabControl().onLanguageChange(args);
    }
}
