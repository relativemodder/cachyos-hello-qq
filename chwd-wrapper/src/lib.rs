use std::ffi::{c_char, CStr};

#[no_mangle]
pub extern "C" fn check_handheld_support() -> i32 {
    let profiles_path = format!(
        "{}/handhelds/profiles.toml",
        chwd::consts::CHWD_PCI_CONFIG_DIR
    );

    let Ok(handheld_profiles) = chwd::profile::parse_profiles(&profiles_path) else {
        return -1; // error
    };

    let handheld_profile_names: Vec<_> = handheld_profiles.iter().map(|p| &p.name).collect();
    let available_profiles = chwd::profile::get_available_profiles(false);

    if available_profiles
        .iter()
        .any(|p| handheld_profile_names.contains(&&p.name))
    {
        return 1; // unsupported hw
    }

    0 // ok
}
