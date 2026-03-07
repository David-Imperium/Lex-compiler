#include "license.hpp"
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace lex::license {

// ============================================================================
// License Feature Flags
// ============================================================================

bool License::has_advanced_backends() const {
    return tier >= Tier::Core;
}

bool License::has_studio_tools() const {
    return tier >= Tier::Studio;
}

bool License::has_multiplayer() const {
    return tier >= Tier::Net;
}

bool License::has_shader_dsl() const {
    return tier >= Tier::Shader;
}

bool License::has_neural() const {
    return tier >= Tier::Neural;
}

std::string License::tier_name() const {
    switch (tier) {
        case Tier::Free:    return "Free";
        case Tier::Core:    return "Core";
        case Tier::Studio:  return "Studio";
        case Tier::Net:     return "Net";
        case Tier::Shader:  return "Shader";
        case Tier::Neural:  return "Neural";
        case Tier::Engine:  return "Engine";
        default:            return "Unknown";
    }
}

std::unordered_set<std::string> License::get_available_backends() const {
    std::unordered_set<std::string> backends = FREE_BACKENDS;

    // Core tier - Engine backends
    if (tier >= Tier::Core) {
        backends.insert(CORE_BACKENDS.begin(), CORE_BACKENDS.end());
    }

    // Net tier - RPC backends
    if (tier >= Tier::Net) {
        backends.insert(NET_BACKENDS.begin(), NET_BACKENDS.end());
    }

    // Shader tier - Shader DSL backends
    if (tier >= Tier::Shader) {
        backends.insert(SHADER_BACKENDS.begin(), SHADER_BACKENDS.end());
    }

    // Neural tier - AI backends
    if (tier >= Tier::Neural) {
        backends.insert(NEURAL_BACKENDS.begin(), NEURAL_BACKENDS.end());
    }

    return backends;
}

// ============================================================================
// License Manager
// ============================================================================

Tier LicenseManager::tier_from_string(std::string_view str) {
    if (str == "FREE" || str == "free")    return Tier::Free;
    if (str == "CORE" || str == "core")    return Tier::Core;
    if (str == "STUDIO" || str == "studio") return Tier::Studio;
    if (str == "NET" || str == "net")      return Tier::Net;
    if (str == "SHADER" || str == "shader") return Tier::Shader;
    if (str == "NEURAL" || str == "neural") return Tier::Neural;
    if (str == "ENGINE" || str == "engine") return Tier::Engine;
    return Tier::Unknown;
}

std::optional<License> LicenseManager::from_env() {
    const char* env_key = std::getenv("LEX_LICENSE");
    if (!env_key || env_key[0] == '\0') {
        return std::nullopt;
    }
    return parse(env_key);
}

std::optional<License> LicenseManager::parse(std::string_view key) {
    // Key format: LEX-{TIER}-{XXXX}-{XXXX}-{XXXX}
    // Example: LEX-CORE-A3F2-B7D9-C1E4

    if (key.empty()) {
        return std::nullopt;
    }

    // Convert to uppercase for parsing
    std::string key_upper;
    key_upper.reserve(key.size());
    for (char c : key) {
        key_upper.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
    }

    // Split by '-'
    std::vector<std::string> parts;
    std::stringstream ss(key_upper);
    std::string part;
    while (std::getline(ss, part, '-')) {
        parts.push_back(part);
    }

    // Validate format: LEX + TIER + 3 random segments
    if (parts.size() != 5) {
        return std::nullopt;
    }

    // Validate prefix
    if (parts[0] != "LEX") {
        return std::nullopt;
    }

    // Validate tier
    Tier tier = tier_from_string(parts[1]);
    if (tier == Tier::Unknown) {
        return std::nullopt;
    }

    // Validate random segments (alphanumeric, 4 chars each)
    for (size_t i = 2; i < 5; ++i) {
        if (parts[i].length() != 4) {
            return std::nullopt;
        }
        for (char c : parts[i]) {
            if (!std::isalnum(static_cast<unsigned char>(c))) {
                return std::nullopt;
            }
        }
    }

    // Validate checksum (last segment)
    // TODO: Implement proper checksum validation
    // For now, just check format

    License license;
    license.tier = tier;
    license.key = key_upper;
    license.random_part = parts[2] + "-" + parts[3] + "-" + parts[4];
    license.is_lifetime = true;  // Early bird licenses are lifetime
    license.is_valid = true;

    // Set expiry (lifetime = far future)
    license.expires_at = std::chrono::system_clock::now() + std::chrono::hours(24 * 365 * 100);

    return license;
}

bool LicenseManager::verify_signature(const License& license) {
    // TODO: Implement Ed25519 signature verification
    // For now, trust the parsed license
    return license.is_valid;
}

bool LicenseManager::is_expired(const License& license) {
    if (license.is_lifetime) {
        return false;
    }
    return std::chrono::system_clock::now() > license.expires_at;
}

bool LicenseManager::validate_checksum(std::string_view key) {
    // TODO: Implement checksum validation
    // Format: CRC32 of prefix + random segments
    return !key.empty();
}

} // namespace lex::license
