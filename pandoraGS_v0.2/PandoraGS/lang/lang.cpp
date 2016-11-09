/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
File name :   lang.h
Description : language resources
*******************************************************************************/
using namespace std;
#include "lang.h"


// -- INGAME MENU LANGUAGE RESOURCE -- -----------------------------------------

/// <summary>Set game language values (necessary)</summary>
/// <param name="code">Language code</param>
void LanguageGameMenuResource::setLanguage(LangCode code) 
{
    // language factory
    switch (code)
    {
        case LangCode_English: setLanguage_English(); break;
        case LangCode_Spanish: setLanguage_Spanish(); break;
        case LangCode_French:  setLanguage_French();  break;
        case LangCode_German:  setLanguage_German();  break;
        case LangCode_CustomFile: setLanguageFromFile(); break;
        default: setLanguage_English(); break;
    }
}

/// <summary>Set english values</summary>
void LanguageGameMenuResource::setLanguage_English()
{
    
}
/// <summary>Set spanish values</summary>
void LanguageGameMenuResource::setLanguage_Spanish()
{

}
/// <summary>Set french values</summary>
void LanguageGameMenuResource::setLanguage_French()
{

}
/// <summary>Set german values</summary>
void LanguageGameMenuResource::setLanguage_German()
{

}

/// <summary>Read values from file (english if not found)</summary>
void LanguageGameMenuResource::setLanguageFromFile()
{

}


// -- DIALOG LANGUAGE RESOURCE -- ----------------------------------------------

/// <summary>Create uninitialized instance</summary>
LanguageDialogResource::LanguageDialogResource() 
{
}
/// <summary>Set dialog language values (necessary)</summary>
/// <param name="code">Language code</param>
void LanguageDialogResource::setLanguage(LangCode code) 
{
    // language factory
    switch (code)
    {
        case LangCode_English: setLanguage_English(); break;
        case LangCode_Spanish: setLanguage_Spanish(); break;
        case LangCode_French:  setLanguage_French();  break;
        case LangCode_German:  setLanguage_German();  break;
        case LangCode_CustomFile: setLanguageFromFile(); break;
        default: setLanguage_English(); break;
    }
}

/// <summary>Set english values</summary>
void LanguageDialogResource::setLanguage_English()
{
    dialog_ok = L"OK";
    dialog_cancel = L"Cancel";
    dialog_profiles = L"Current profile:";
    tabTitle_generalSettings = L"General settings";
    tabTitle_profilesManager = L"Profile manager";
    tabTitle_profileSettings = L"Profile settings";

    manager_tableColumnProfile = L"profile";
    manager_btnAdd_tooltip = L"Add a new profile";
    manager_btnEdit_tooltip = L"Edit the selected profile (name and associations)";
    manager_btnRemove_tooltip = L"Remove selected profiles";
    manager_btnImport_tooltip = L"Import profile (from file)";
    manager_btnExport_tooltip = L"Export profile (save file)";
    manager_presets = L"Profile presets";
    manager_btnPresetsApply = L"Apply to selection";
    manager_preset_fastest = L"Fastest";
    manager_preset_standard = L"Standard";
    manager_preset_enhanced2d = L"Enhanced 2D";
    manager_preset_enhanced3d = L"Enhanced 3D";

    profile_internal_resx[0] = L"X: native (1x)";
    profile_internal_resx[1] = L"X: standard (2x)";
    profile_internal_resx[2] = L"X: high (4x)";
    profile_internal_resx[3] = L"X: higher (6x, slow)";
    profile_internal_resx[4] = L"X: ultra (8x, slow)";
    profile_internal_resy[0] = L"Y: native (1x)";
    profile_internal_resy[1] = L"Y: standard (2x)";
    profile_internal_resy[2] = L"Y: medium (4x)";
    profile_internal_resy[3] = L"Y: high (8x)";
    profile_internal_resy[4] = L"Y: higher (10x, slow)";
    profile_internal_resy[5] = L"Y: ultra (12x, slow)";
    profile_stretch_presets[0] = L"Custom settings";
    profile_stretch_presets[1] = L"Full window (stretched)";
    profile_stretch_presets[2] = L"Keep ratio (best with 2D)";
    profile_stretch_presets[3] = L"Keep ratio & crop (fill)";
    profile_stretch_presets[4] = L"Half stretched/cropped";
}
/// <summary>Set spanish values</summary>
void LanguageDialogResource::setLanguage_Spanish()
{
    dialog_ok = L"OK";
    dialog_cancel = L"Cancelar";
    dialog_profiles = L"Current profile:";
    tabTitle_generalSettings = L"General settings";
    tabTitle_profilesManager = L"Profile manager";
    tabTitle_profileSettings = L"Profile settings";

    manager_tableColumnProfile = L"profile";
    manager_btnAdd_tooltip = L"Add a new profile";
    manager_btnEdit_tooltip = L"Edit the selected profile (name and associations)";
    manager_btnRemove_tooltip = L"Remove selected profiles";
    manager_btnImport_tooltip = L"Import profile (from file)";
    manager_btnExport_tooltip = L"Export profile (save file)";
    manager_presets = L"Profile presets";
    manager_btnPresetsApply = L"Apply to selection";
    manager_preset_fastest = L"Fastest";
    manager_preset_standard = L"Standard";
    manager_preset_enhanced2d = L"Enhanced 2D";
    manager_preset_enhanced3d = L"Enhanced 3D";

    profile_internal_resx[0] = L"X: native (1x)";
    profile_internal_resx[1] = L"X: standard (2x)";
    profile_internal_resx[2] = L"X: high (4x)";
    profile_internal_resx[3] = L"X: higher (6x, slow)";
    profile_internal_resx[4] = L"X: ultra (8x, slow)";
    profile_internal_resy[0] = L"Y: native (1x)";
    profile_internal_resy[1] = L"Y: standard (2x)";
    profile_internal_resy[2] = L"Y: medium (4x)";
    profile_internal_resy[3] = L"Y: high (8x)";
    profile_internal_resy[4] = L"Y: higher (10x, slow)";
    profile_internal_resy[5] = L"Y: ultra (12x, slow)";
    profile_stretch_presets[0] = L"Custom settings";
    profile_stretch_presets[1] = L"Full window (stretched)";
    profile_stretch_presets[2] = L"Keep ratio (best with 2D)";
    profile_stretch_presets[3] = L"Keep ratio & crop (fill)";
    profile_stretch_presets[4] = L"Half stretched/cropped";
}
/// <summary>Set french values</summary>
void LanguageDialogResource::setLanguage_French()
{
    dialog_ok = L"OK";
    dialog_cancel = L"Annuler";
    dialog_profiles = L"Profil courant :";
    tabTitle_generalSettings = L"Options g�n�rales";
    tabTitle_profilesManager = L"Gestion de profils";
    tabTitle_profileSettings = L"Param�trer profil";

    manager_tableColumnProfile = L"profil";
    manager_btnAdd_tooltip = L"Ajouter un profil";
    manager_btnEdit_tooltip = L"Editer le profil s�lectionn� (nom et associations)";
    manager_btnRemove_tooltip = L"Supprimer la s�lection";
    manager_btnImport_tooltip = L"Importer profil (depuis fichier)";
    manager_btnExport_tooltip = L"Exporter profil (sauvegarder)";
    manager_presets = L"Pr�configurer profil";
    manager_btnPresetsApply = L"Modifier s�lection";
    manager_preset_fastest = L"Plus rapide";
    manager_preset_standard = L"Standard";
    manager_preset_enhanced2d = L"2D am�lior�e";
    manager_preset_enhanced3d = L"3D am�lior�e";

    profile_internal_resx[0] = L"X: native (1x)";
    profile_internal_resx[1] = L"X: standard (2x)";
    profile_internal_resx[2] = L"X: haute (4x)";
    profile_internal_resx[3] = L"X: haute+ (6x, lent)";
    profile_internal_resx[4] = L"X: ultra (8x, lent)";
    profile_internal_resy[0] = L"Y: native (1x)";
    profile_internal_resy[1] = L"Y: standard (2x)";
    profile_internal_resy[2] = L"Y: medium (4x)";
    profile_internal_resy[3] = L"Y: haute (8x)";
    profile_internal_resy[4] = L"Y: haute+ (10x, lent)";
    profile_internal_resy[5] = L"Y: ultra (12x, lent)";
    profile_stretch_presets[0] = L"Personnalis�";
    profile_stretch_presets[1] = L"Remplir fen�tre (�tirer)";
    profile_stretch_presets[2] = L"Conserver ratio (id�al 2D)";
    profile_stretch_presets[3] = L"Conserver ratio & tronquer";
    profile_stretch_presets[4] = L"Semi �tir�/tronqu�";
}
/// <summary>Set german values</summary>
void LanguageDialogResource::setLanguage_German()
{
    dialog_ok = L"OK";
    dialog_cancel = L"Abbrechen";
    dialog_profiles = L"Current profile:";
    tabTitle_generalSettings = L"General settings";
    tabTitle_profilesManager = L"Profile manager";
    tabTitle_profileSettings = L"Profile settings";

    manager_tableColumnProfile = L"profile";
    manager_btnAdd_tooltip = L"Add a new profile";
    manager_btnEdit_tooltip = L"Edit the selected profile (name and associations)";
    manager_btnRemove_tooltip = L"Remove selected profiles";
    manager_btnImport_tooltip = L"Import profile (from file)";
    manager_btnExport_tooltip = L"Export profile (save file)";
    manager_presets = L"Profile presets";
    manager_btnPresetsApply = L"Apply to selection";
    manager_preset_fastest = L"Fastest";
    manager_preset_standard = L"Standard";
    manager_preset_enhanced2d = L"Enhanced 2D";
    manager_preset_enhanced3d = L"Enhanced 3D";

    profile_internal_resx[0] = L"X: native (1x)";
    profile_internal_resx[1] = L"X: standard (2x)";
    profile_internal_resx[2] = L"X: high (4x)";
    profile_internal_resx[3] = L"X: higher (6x, slow)";
    profile_internal_resx[4] = L"X: ultra (8x, slow)";
    profile_internal_resy[0] = L"Y: native (1x)";
    profile_internal_resy[1] = L"Y: standard (2x)";
    profile_internal_resy[2] = L"Y: medium (4x)";
    profile_internal_resy[3] = L"Y: high (8x)";
    profile_internal_resy[4] = L"Y: higher (10x, slow)";
    profile_internal_resy[5] = L"Y: ultra (12x, slow)";
    profile_stretch_presets[0] = L"Custom settings";
    profile_stretch_presets[1] = L"Full window (stretched)";
    profile_stretch_presets[2] = L"Keep ratio (best with 2D)";
    profile_stretch_presets[3] = L"Keep ratio & crop (fill)";
    profile_stretch_presets[4] = L"Half stretched/cropped";
}

/// <summary>Read values from file</summary>
/// <exception cref="std::exception">File not found</exception>
void LanguageDialogResource::setLanguageFromFile()
{
    setLanguage_English();
    //...read file Config::gen_langFilePath
}
