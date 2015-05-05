package haxebridge;

import java.util.Map;
import java.util.HashMap;

class Bridge {

    static {
        System.loadLibrary("haxebridge");
    }

        // Channel listener interface
    public static interface ChannelListener {
            // Called when a message is received from haxe
        public String onMessageReceived(String channel, String message);

    }

    private static Map<String,ChannelListener> sChannelListeners = null;
    private static Map<String,ChannelListener> getChannelListeners() {
        if (sChannelListeners == null) {
            sChannelListeners = new HashMap<String,ChannelListener>();
        }
        return sChannelListeners;
    }

        // Send message from java to haxe
    public static native String sendMessage(String channel, String message);

        // Listen to messages sent from haxe to java
    public static void listenChannel(String channel, ChannelListener listener) {
        getChannelListeners().put(channel, listener);
    }

        // Init native listener
    private static native void initNativeListener();

        // Receive message from haxe
    private static String receiveFromHaxe(String channel, String message) {
        ChannelListener listener = getChannelListeners().get(channel);
        if (listener != null) {
            return listener.onMessageReceived(channel, message);
        }
        return null;
    }

}
