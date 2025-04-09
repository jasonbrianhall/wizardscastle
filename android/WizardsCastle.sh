#!/bin/bash
set -e

# Check for NDK
if [ -z "$ANDROID_NDK_HOME" ]; then
	echo "Error: ANDROID_NDK_HOME not set (download the Android NDK, extract it and set this variable)"
    exit 1
fi

# Check for NDK
if [ -z "$ANDROID_HOME" ]; then
    echo "Error: ANDROID_HOME not set (e.g. /home/user/Android/Sdk is using Android Studio)"
    exit 1
fi


# Check for ImageMagick
if ! command -v convert &> /dev/null; then
    echo "Error: ImageMagick not found (needed for icon conversion)"
    exit 1
fi

# Check for ImageMagick
if ! command -v gradle &> /dev/null; then
    echo -en "Warning: Command gradle not found (used to compile the embedded Java)\n\n"
fi


# Create project structure
mkdir -p WizardsCastle/app/src/main/{assets,java/org/wizardscastle/terminalwizcastle,res}
cd WizardsCastle

# Create resource directories
mkdir -p app/src/main/res/mipmap-{mdpi,hdpi,xhdpi,xxhdpi,xxxhdpi}
mkdir -p app/src/main/res/mipmap-anydpi-v26
mkdir -p app/src/main/res/drawable

# Set up ImageMagick quality settings
CONVERT_OPTS="-strip -quality 100 -define png:compression-level=9"

# Convert main icon for each density with high quality settings
convert $CONVERT_OPTS ../icon.png -resize 192x192 app/src/main/res/mipmap-mdpi/ic_launcher.png
convert $CONVERT_OPTS ../icon.png -resize 288x288 app/src/main/res/mipmap-hdpi/ic_launcher.png
convert $CONVERT_OPTS ../icon.png -resize 384x384 app/src/main/res/mipmap-xhdpi/ic_launcher.png
convert $CONVERT_OPTS ../icon.png -resize 576x576 app/src/main/res/mipmap-xxhdpi/ic_launcher.png
convert $CONVERT_OPTS ../icon.png -resize 768x768 app/src/main/res/mipmap-xxxhdpi/ic_launcher.png

# Create foreground layer icons (75% of main icon size for padding)
convert $CONVERT_OPTS ../icon.png -resize 144x144 app/src/main/res/mipmap-mdpi/ic_launcher_foreground.png
convert $CONVERT_OPTS ../icon.png -resize 216x216 app/src/main/res/mipmap-hdpi/ic_launcher_foreground.png
convert $CONVERT_OPTS ../icon.png -resize 288x288 app/src/main/res/mipmap-xhdpi/ic_launcher_foreground.png
convert $CONVERT_OPTS ../icon.png -resize 432x432 app/src/main/res/mipmap-xxhdpi/ic_launcher_foreground.png
convert $CONVERT_OPTS ../icon.png -resize 576x576 app/src/main/res/mipmap-xxxhdpi/ic_launcher_foreground.png

# Create round icons (same dimensions as regular icons)
convert $CONVERT_OPTS ../icon.png -resize 192x192 app/src/main/res/mipmap-mdpi/ic_launcher_round.png
convert $CONVERT_OPTS ../icon.png -resize 288x288 app/src/main/res/mipmap-hdpi/ic_launcher_round.png
convert $CONVERT_OPTS ../icon.png -resize 384x384 app/src/main/res/mipmap-xhdpi/ic_launcher_round.png
convert $CONVERT_OPTS ../icon.png -resize 576x576 app/src/main/res/mipmap-xxhdpi/ic_launcher_round.png
convert $CONVERT_OPTS ../icon.png -resize 768x768 app/src/main/res/mipmap-xxxhdpi/ic_launcher_round.png

# Create splash icon drawable from the main icon
convert $CONVERT_OPTS ../icon_splash.png -resize 512x512 app/src/main/res/drawable/splash_icon.png



# Create background resource
cat > app/src/main/res/drawable/ic_launcher_background.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<shape xmlns:android="http://schemas.android.com/apk/res/android">
    <solid android:color="#FFFFFF"/>
</shape>
EOL

# Create adaptive icon config
cat > app/src/main/res/mipmap-anydpi-v26/ic_launcher.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<adaptive-icon xmlns:android="http://schemas.android.com/apk/res/android">
    <background android:drawable="@drawable/ic_launcher_background"/>
    <foreground android:drawable="@mipmap/ic_launcher_foreground"/>
</adaptive-icon>
EOL

# Create round adaptive icon config
cat > app/src/main/res/mipmap-anydpi-v26/ic_launcher_round.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<adaptive-icon xmlns:android="http://schemas.android.com/apk/res/android">
    <background android:drawable="@drawable/ic_launcher_background"/>
    <foreground android:drawable="@mipmap/ic_launcher_foreground"/>
</adaptive-icon>
EOL

# Copy and compile the C program for each architecture
cp ../../c/*.c app/src/main/assets/
cp ../../c/*.h app/src/main/assets/
#cp ../*.c app/src/main/assets/

for arch in arm64-v8a x86_64; do
    mkdir -p app/src/main/jniLibs/$arch
    case $arch in
        "arm64-v8a") 
            target="aarch64-linux-android21"
            ;;
        "x86_64") 
            target="x86_64-linux-android21"
            ;;
    esac
    
    $ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/clang \
        --target=$target \
        -fPIE \
        -fPIC \
        -static \
        -Wl,--icf=all \
        -Wl,-z,max-page-size=4096 \
        -Wl,-z,relro \
        -Wl,--build-id \
        -Wl,--no-undefined \
        -Wl,-z,noexecstack \
        -Wl,--gc-sections \
        -o app/src/main/jniLibs/$arch/libwizcastle.so \
        app/src/main/assets/*.c
done

rm app/src/main/assets/*.c
rm app/src/main/assets/*.h

# Create SplashActivity.java
cat > app/src/main/java/org/wizardscastle/terminalwizcastle/SplashActivity.java << 'EOL'
package org.wizardscastle.terminalwizcastle;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;

public class SplashActivity extends Activity {
    private static final int SPLASH_DURATION = 2000; // 2 seconds

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Remove title bar
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                           WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        setContentView(R.layout.activity_splash);

        new Handler().postDelayed(() -> {
            Intent intent = new Intent(SplashActivity.this, MainActivity.class);
            startActivity(intent);
            finish();
        }, SPLASH_DURATION);
    }
}
EOL

# Create splash screen layout
mkdir -p app/src/main/res/layout
cat > app/src/main/res/layout/activity_splash.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="@android:color/black">

    <ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_centerInParent="true"
        android:src="@drawable/splash_icon"
        android:contentDescription="@string/app_name"/>
</RelativeLayout>
EOL

# Create TerminalView.java
cat > app/src/main/java/org/wizardscastle/terminalwizcastle/TerminalView.java << 'EOL'
@Override
public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
    outAttrs.inputType = EditorInfo.TYPE_CLASS_TEXT 
                       | EditorInfo.TYPE_TEXT_FLAG_AUTO_CORRECT 
                       | EditorInfo.TYPE_TEXT_FLAG_AUTO_COMPLETE;
    outAttrs.imeOptions = EditorInfo.IME_ACTION_NONE 
                        | EditorInfo.IME_FLAG_NO_EXTRACT_UI 
                        | EditorInfo.IME_FLAG_NO_FULLSCREEN;
    
    return new BaseInputConnection(this, true) {
        @Override
        public boolean commitText(CharSequence text, int newCursorPosition) {
            if (outputStream != null) {
                try {
                    String str = text.toString();
                    inputBuffer.append(str);
                    write(str.getBytes());  // Show the character
                    outputStream.flush();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return true;
        }
        
        @Override
        public boolean setComposingText(CharSequence text, int newCursorPosition) {
            // For predictive text support
            return commitText(text, newCursorPosition);
        }
        
        @Override
        public boolean setComposingRegion(int start, int end) {
            // Required for predictive text
            return true;
        }
        
        @Override
        public boolean finishComposingText() {
            // Required for predictive text
            return true;
        }
    };
}
EOL

# Create MainActivity.java
cat > app/src/main/java/org/wizardscastle/terminalwizcastle/MainActivity.java << 'EOL'
package org.wizardscastle.terminalwizcastle;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.view.View;
import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;

public class MainActivity extends Activity {
    private static final String TAG = "WizardsCastle";
    private TerminalView terminalView;
    private Process process;
    private Thread outputThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Create main layout
        FrameLayout layout = new FrameLayout(this);
        
        // Create terminal view
        terminalView = new TerminalView(this);
        layout.addView(terminalView);
        
        // Create keyboard toggle button
        ImageButton keyboardButton = new ImageButton(this);
        keyboardButton.setImageResource(R.drawable.ic_keyboard);
        keyboardButton.setBackgroundColor(0x80000000);  // Semi-transparent black
        keyboardButton.setPadding(20, 20, 20, 20);
        
        // Set button layout parameters
        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT,
            Gravity.BOTTOM | Gravity.END
        );
        params.setMargins(20, 20, 20, 20);
        keyboardButton.setLayoutParams(params);
        
        // Add button click listener
        keyboardButton.setOnClickListener(v -> terminalView.toggleKeyboard());
        
        // Add button to layout
        layout.addView(keyboardButton);
        
        setContentView(layout);
        
        // Initialize terminal
        terminalView.post(() -> {
            terminalView.requestFocus();
            terminalView.showKeyboard();
        });
        
        startProcess();
    }
    
    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        // We won't actually save any state here since the terminal buffers are managed separately
    }

    @Override
    protected void onPause() {
        super.onPause();
        // Don't destroy the process when paused
    }

    @Override
    protected void onResume() {
        super.onResume();
        // If process died, restart it
        if (process == null || !isProcessAlive()) {
            startProcess();
        }
    }

    private boolean isProcessAlive() {
        if (process == null) return false;
        try {
            process.exitValue();
            return false; // If we get here, the process has exited
        } catch (IllegalThreadStateException e) {
            return true; // Process is still running
        }
    }
    
    private void startProcess() {
        try {
            String abi = getabi();
            Log.d(TAG, "Using ABI: " + abi);
            
            File nativeLibDir = new File(getApplicationInfo().nativeLibraryDir);
            File binFile = new File(nativeLibDir, "libwizcastle.so");
            Log.d(TAG, "Binary path: " + binFile.getAbsolutePath());
            
            ProcessBuilder pb = new ProcessBuilder(binFile.getAbsolutePath());
            pb.directory(getFilesDir());
            pb.environment().put("TERM", "dumb");
            pb.environment().put("HOME", getFilesDir().getAbsolutePath());
            pb.environment().put("TMPDIR", getCacheDir().getAbsolutePath());
            pb.redirectErrorStream(true);
            
            Log.d(TAG, "Starting process...");
            process = pb.start();
            
            final OutputStream processInput = process.getOutputStream();
            final InputStream processOutput = process.getInputStream();
            
            terminalView.setOutputStream(processInput);
            
            outputThread = new Thread(() -> {
                byte[] buffer = new byte[4096];
                try {
                    while (true) {
                        int available = processOutput.available();
                        if (available > 0) {
                            int read = processOutput.read(buffer, 0, Math.min(available, buffer.length));
                            if (read > 0) {
                                final byte[] data = new byte[read];
                                System.arraycopy(buffer, 0, data, 0, read);
                                runOnUiThread(() -> terminalView.write(data));
                            }
                        }
                        
                        try {
                            int exitCode = process.exitValue();
                            runOnUiThread(() -> terminalView.write(
                                ("\nProcess exited with code " + exitCode + "\n").getBytes()));
                            break;
                        } catch (IllegalThreadStateException e) {
                        }
                        
                        Thread.sleep(10);
                    }
                } catch (Exception e) {
                    final String error = "Error reading output: " + e.getMessage() + "\n";
                    runOnUiThread(() -> terminalView.write(error.getBytes()));
                }
            });
            outputThread.start();
            
        } catch (Exception e) {
            Log.e(TAG, "Error in startProcess", e);
            String error = "Error: " + e.getMessage() + "\n";
            terminalView.write(error.getBytes());
        }
    }
    
    private String getabi() {
        String[] abis = android.os.Build.SUPPORTED_ABIS;
        StringBuilder abiList = new StringBuilder("Available ABIs: ");
        for (String abi : abis) {
            abiList.append(abi).append(" ");
        }
        Log.d(TAG, abiList.toString());
        
        for (String abi : abis) {
            if (abi.equals("x86_64")) return "x86_64";
            if (abi.equals("arm64-v8a")) return "arm64-v8a";
        }
        return "x86_64";
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (process != null) {
            process.destroy();
        }
        if (outputThread != null) {
            outputThread.interrupt();
        }
    }
}
EOL

cat > app/src/main/res/drawable/ic_keyboard.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<vector xmlns:android="http://schemas.android.com/apk/res/android"
    android:width="24dp"
    android:height="24dp"
    android:viewportWidth="24"
    android:viewportHeight="24">
    <path
        android:fillColor="#FFFFFF"
        android:pathData="M20,5L4,5c-1.1,0 -1.99,0.9 -1.99,2L2,17c0,1.1 0.9,2 2,2h16c1.1,0 2,-0.9 2,-2L22,7c0,-1.1 -0.9,-2 -2,-2zM11,8h2v2h-2L11,8zM11,11h2v2h-2v-2zM8,8h2v2L8,10L8,8zM8,11h2v2L8,13v-2zM7,13L5,13v-2h2v2zM7,10L5,10L5,8h2v2zM16,17L8,17v-2h8v2zM16,13h-2v-2h2v2zM16,10h-2L14,8h2v2zM19,13h-2v-2h2v2zM19,10h-2L17,8h2v2z"/>
</vector>
EOL

# Create empty proguard-rules.pro
touch app/proguard-rules.pro

# Create strings.xml
mkdir -p app/src/main/res/values
cat > app/src/main/res/values/strings.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="app_name">Wizard\'s Castle</string>
    <string name="app_description">Wizards Castle is a classic text adventure game where you explore a mysterious castle filled with treasures and dangers. Navigate through rooms, battle monsters, and collect treasures as you try to survive this perilous dungeon crawl in order to collect the Orb of Zot.  Based on the 1980 BASIC game of the same name with creative differences.</string>
    <string name="developer">Jason Hall</string>
    <string name="developer_email">jasonbrianhall@gmail.com</string>
    <string name="website_url">https://github.com/jasonbrianhall/wizardscastle</string>
    <string name="source_license">MIT License</string>
    <string name="game_features">Randomly generated 512-room castles, Multiple character races (Human, Elf, Hobbit, Dwarf, Dark Elf), Strategic combat system, Magic spells, Vendor interactions, ASCII map display</string>
    <string name="original_concept">Joseph R. Power (1980)</string> 
    <string name="app_icon">App Icon</string>
</resources>
EOL

# Update AndroidManifest.xml
cat > app/src/main/AndroidManifest.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    <application 
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:roundIcon="@mipmap/ic_launcher_round"
        android:label="@string/app_name">
        
        <!-- Meta-data using string resources -->
        <meta-data
            android:name="developer"
            android:value="@string/developer" />

        <meta-data
            android:name="developer_email"
            android:value="@string/developer_email" />
            
        <meta-data
            android:name="website_url"
            android:value="@string/website_url" />
            
        <meta-data
            android:name="game_description"
            android:value="@string/app_description" />

        <meta-data
            android:name="source_license"
            android:value="@string/source_license" />

        <meta-data
            android:name="game_features"
            android:value="@string/game_features" />

        <meta-data
            android:name="original_concept"
            android:value="@string/original_concept" />

        <activity
            android:name=".SplashActivity"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
            android:exported="true">
           <intent-filter>
               <action android:name="android.intent.action.MAIN" />
               <category android:name="android.intent.category.LAUNCHER" />
               </intent-filter>
        </activity>

        <activity 
            android:name=".MainActivity" 
            android:exported="true"
            android:configChanges="orientation|screenSize|screenLayout|keyboardHidden"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
            android:windowSoftInputMode="adjustResize">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
EOL

# Create build.gradle
cat > app/build.gradle << 'EOL'
plugins {
    id 'com.android.application'
}

android {
    namespace 'org.wizardscastle.terminalwizcastle'
    compileSdkVersion 33
    
    defaultConfig {
        applicationId "org.wizardscastle.terminalwizcastle"
        minSdkVersion 21
        targetSdkVersion 33
        versionCode CHANGEME1
        versionName CHANGEME2
    }
    
    buildTypes {
        release {
            minifyEnabled true
            shrinkResources true
	    signingConfig signingConfigs.debug  // Use debug signing for testing
        }
    }   
 
    applicationVariants.all { variant ->
        variant.outputs.all {
            outputFileName = "wizards-castle.apk"
        }
    }
}
EOL

VERSION_CODE=$(date +%Y%m%d)
VERSION_NAME=$(date +%Y.%m.%d)

sed "s/CHANGEME1/$VERSION_CODE/g" app/build.gradle -i
sed "s/CHANGEME2/\"$VERSION_NAME\"/g" app/build.gradle -i

# Create settings.gradle
cat > settings.gradle << 'EOL'
pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.PREFER_SETTINGS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = 'WizardsCastle'
include ':app'
EOL

# Create root build.gradle
cat > build.gradle << 'EOL'
buildscript {
    repositories {
        google()
        mavenCentral()
    }
    dependencies {
        classpath 'com.android.tools.build:gradle:8.1.0'
    }
}
EOL

# Create gradle.properties
cat > gradle.properties << 'EOL'
org.gradle.jvmargs=-Xmx2048m -Dfile.encoding=UTF-8
android.useAndroidX=true
android.nonTransactiveRClass=true
android.suppressUnsupportedCompileSdk=34
EOL

echo "Project created. To build:"
echo "1. Make sure ANDROID_NDK_HOME and ANDROID_HOME is set and correct"
echo "2. cd WizardsCastle"
echo "3. gradle assembleDebug (Debug) or gradle assemble"
echo "The APK will be in app/build/outputs/apk/debug/wizards-castle.apk"
echo -en "\n\nFor bundleRelease for the Google Playstore, run the command ./gradlew bundleRelease -PKEYSTORE_PASSWORD=xxx -PKEY_ALIAS=googleplay -PKEY_PASSWORD=xxx and place the release-key.jks in the app directory\n\n"
