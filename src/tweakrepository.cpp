#include "tweakrepository.h"

TweakRepository& TweakRepository::instance()
{
    static TweakRepository instance;
    return instance;
}

TweakRepository::TweakRepository()
{
    initializeTweaks();
}

void TweakRepository::initializeTweaks()
{
    m_tweaks = {
        {"psd", {
            "psd",
            "Profile-sync-daemon",
            "Manages browser profiles in tmpfs for reduced disk I/O",
            TweakDefinition::UserService,
            {"psd.service"},
            "profile-sync-daemon"
        }},

        {"oomd", {
            "oomd",
            "Systemd-oomd",
            "Userspace out-of-memory killer",
            TweakDefinition::SystemService,
            {"systemd-oomd.service"},
            "systemd-oomd"
        }},

        {"bpftune", {
            "bpftune",
            "Bpftune",
            "Auto-tuning system performance using BPF",
            TweakDefinition::SystemService,
            {"bpftune.service"},
            "bpftune"
        }},

        {"bluetooth", {
            "bluetooth",
            "Bluetooth",
            "Bluetooth support",
            TweakDefinition::SystemService,
            {"bluetooth.service"},
            "bluez"
        }},

        {"ananicy", {
            "ananicy",
            "Ananicy Cpp",
            "Auto nice daemon with community rules",
            TweakDefinition::SystemService,
            {"ananicy-cpp.service"},
            "ananicy-cpp"
        }},

        {"cachy-update", {
            "cachy-update",
            "CachyOS Update Notifier",
            "System update notifications in tray",
            TweakDefinition::UserService,
            {"arch-update.timer", "arch-update-tray.service"},
            "cachy-update"
        }}
    };
}

TweakDefinition TweakRepository::getTweak(const QString& id) const
{
    return m_tweaks.value(id);
}

QList<TweakDefinition> TweakRepository::getAllTweaks() const
{
    return m_tweaks.values();
}

bool TweakRepository::exists(const QString& id) const
{
    return m_tweaks.contains(id);
}
