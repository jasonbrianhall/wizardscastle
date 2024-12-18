# 🏰 The Wizard's Castle - Android Port

*A mystical journey to port an ancient spell into the realm of Android devices*

## 📜 Quest Overview

Brave adventurer! You seek to compile the legendary Wizard's Castle for Android devices. This sacred tome contains the knowledge required to embark on your compilation quest. Follow these instructions carefully, lest you fall prey to the dreaded "Build Failed" demon.

## 🧙‍♂️ Prerequisites - Gathering Your Equipment

Before beginning your journey, you must obtain these mystical artifacts:

### Required Artifacts of Power

1. **Android SDK** (The Foundation Stone)
   - Journey to the [Android Studio Download Portal](https://developer.android.com/studio)
   - Install Android Studio, which contains the sacred SDK
   - The SDK typically materializes in `~/Android/Sdk` (Linux/Mac) or `C:\Users\[Username]\AppData\Local\Android\Sdk` (Windows)

2. **Android NDK** (The Native Enchanter)
   - Launch Android Studio
   - Navigate to Tools → SDK Manager
   - Select "SDK Tools" tab
   - Check "NDK (Side by side)" and apply the changes
   - The NDK shall appear in `[Android SDK Location]/ndk/[version]`

3. **ImageMagick** (The Image Transmuter)
   - *Ubuntu/Debian:* Cast `sudo apt-get install imagemagick`
   - *Mac:* Invoke `brew install imagemagick`
   - *Windows:* Download from the [ImageMagick Portal](https://imagemagick.org/script/download.php)

4. **Gradle** (The Build Master)
   - *Ubuntu/Debian:* Summon with `sudo apt-get install gradle`
   - *Mac:* Conjure using `brew install gradle`
   - *Windows:* Download from [Gradle's Realm](https://gradle.org/install/)
   - *Other:* Download from the [internet](https://github.com/gradle/gradle-distributions/releases/download/v8.11.1/gradle-8.11.1-bin.zip), unzip and set the path appropriately to the bin directory

## 🗺️ The Path of Configuration

### Enchanting Your Environment

You must bind these mystical environment variables to your shell configuration (`~/.bashrc`, `~/.zshrc`, or Windows Environment Variables):

```bash
# The location of your Android SDK
export ANDROID_HOME=$HOME/Android/Sdk  # Adjust path as needed

# The location of your Android NDK
export ANDROID_NDK_HOME=$ANDROID_HOME/ndk/[version]  # Replace [version] with your NDK version
```

## 🎯 The Compilation Ritual

1. **Clone the Sacred Repository**
   ```bash
   git clone [your-repository-url]
   cd [repository-directory]
   ```

2. **Invoke the Build Script**
   ```bash
   chmod +x wizardscastle.sh
   ./wizardscastle.sh
   ```

3. **Enter the Castle Directory**
   ```bash
   cd WizardsCastle
   ```

4. **Summon the APK**
   - For a debug build:
     ```bash
     gradle assembleDebug
     ```
   - For a release build:
     ```bash
     gradle assembleRelease or gradle assemble
     ```

5. **Locate Your Treasure**
   The mythical APK will materialize in:
   ```
   app/build/outputs/apk/debug/wizards-castle.apk
   ```

## 🎮 Installing the Enchanted APK

### Method 1: The Direct Enchantment
```bash
adb install app/build/outputs/apk/debug/wizards-castle.apk
```

### Method 2: The Manual Ritual
1. Enable "Unknown Sources" in your Android device's settings
2. Transfer the APK to your device
3. Tap the APK to install

## 🐉 Troubleshooting - Battling Common Demons

### The "Command Not Found" Curse
```bash
Error: Command 'gradle' not found
```
*Solution:* Ensure Gradle is properly installed and added to your PATH.

### The "NDK Not Found" Hex
```bash
Error: ANDROID_NDK_HOME not set
```
*Solution:* Verify your environment variables are correctly set and the NDK is installed.

### The "Missing ImageMagick" Affliction
```bash
Error: ImageMagick not found
```
*Solution:* Install ImageMagick using your system's package manager.

## 📜 Ancient Scrolls (Documentation)

- [Android NDK Documentation](https://developer.android.com/ndk/guides)
- [Gradle Build Tool](https://gradle.org/documentation/)
- [Android Studio User Guide](https://developer.android.com/studio/intro)

## 🎭 Credits

This port was crafted by brave code wizards who dared to bring this classic dungeon crawler to the realm of Android. May their code forever run bug-free.

*"In the realm of Android, where permissions dwell,
This castle stands, with stories to tell.
Compile it right, and you shall see,
A game of wonder, running bug-free!"*

