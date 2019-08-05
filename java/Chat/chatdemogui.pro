-target 1.8
-dontshrink
-dontoptimize
-dontobfuscate
-dontusemixedcaseclassnames
-dontpreverify

-dontnote com.sum.**
-dontnote Ice.**
-dontnote IceInternal.**
-dontnote com.jgoodies.**
-dontnote jdk.internal.jimage.**
-dontnote jdk.internal.jrtfs.**

# Keep - Applications. Keep all application classes, along with their 'main'
# methods.
-keepclasseswithmembers public class * {
    public static void main(java.lang.String[]);
}

# Also keep - Swing UI L&F. Keep all extensions of javax.swing.plaf.ComponentUI,
# along with the special 'createUI' method.
-keep class * extends javax.swing.plaf.ComponentUI {
    public static javax.swing.plaf.ComponentUI createUI(javax.swing.JComponent);
}

# Keep names - Native method names. Keep all native class/method names.
-keepclasseswithmembers,includedescriptorclasses,allowshrinking class * {
    native <methods>;
}
