package com.wpe.wpe.services;

public class NetworkProcessGlue {

    static {
        System.loadLibrary("WPENetworkProcessGlue");
    }

    public static native void initializeXdg(String xdgRuntimePath);
    public static native void initializeGioExtraModulesPath(String extraModulesPath);
    public static native void initializeMain(int fd);
}
