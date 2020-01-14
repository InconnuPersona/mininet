package org.mininet.sdl;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;

import org.libsdl.app.SDLActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Mininet extends SDLActivity {
    private void copyFile(InputStream source, File target) throws IOException {
        OutputStream output;
        byte[] buffer;
        int read;

        buffer = new byte[1024];
        output = new FileOutputStream(target);

        while ((read = source.read(buffer)) != -1) {
            output.write(buffer, 0, read);
        }

        output.flush();
        output.close();
    }

    private void loadAssets(String path) {
        AssetManager manager;
        File file;
        String[] assets;
        InputStream input;
        int i;

        if (path == null) {
            Log.i("tag", "Mininet.loadAssets: received invalid asset path.");
            return;
        }

        assets = null;

        manager = getAssets();

        try {
            assets = manager.list(path);
        }
        catch (IOException e) {
            Log.e("tag", "Mininet.loadAssets: unable to list directory contents.", e);
            return;
        }

        Log.i("tag", "Mininet.loadAssets: loading assets to '" + getFilesDir() + "'");

        try {
            file = new File(getFilesDir() + "/", path);

            if (assets.length == 0) {
                input = manager.open(path);

                copyFile(input, file);

                input.close();
                input = null;
            }
            else {
                if (!file.exists()) {
                    file.mkdir();
                }

                for (i = 0; i < assets.length; i++) {
                    loadAssets(path + "/" + assets[i]);
                }
            }
        }
        catch (IOException e) {
            Log.e("tag", "Mininet.loadAssets: unable to copy asset '" + path + "'.", e);
        }
    }

    @Override
    public void onCreate(Bundle context) {
        loadAssets("res");

        super.onCreate(context);
    }
}