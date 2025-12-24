package util;

import java.security.MessageDigest;
import java.security.SecureRandom;

public final class PasswordUtil {
    private static final int SALT_BYTES = 16;

    private PasswordUtil() {
    }

    public static String generateSaltedHash(String password) throws Exception {
        byte[] salt = generateSalt();
        String saltHex = toHex(salt);
        String hashHex = hash(password, salt);
        return saltHex + ":" + hashHex;
    }

    public static boolean verify(String password, String stored) throws Exception {
        if (stored == null || !stored.contains(":")) {
            return false;
        }
        String[] parts = stored.split(":", 2);
        if (parts.length != 2) {
            return false;
        }
        byte[] salt = fromHex(parts[0]);
        String expectedHash = parts[1];
        String actualHash = hash(password, salt);
        return MessageDigest.isEqual(actualHash.getBytes(), expectedHash.getBytes());
    }

    private static String hash(String password, byte[] salt) throws Exception {
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        md.update(salt);
        byte[] digest = md.digest(password.getBytes("UTF-8"));
        return toHex(digest);
    }

    private static byte[] generateSalt() {
        byte[] salt = new byte[SALT_BYTES];
        new SecureRandom().nextBytes(salt);
        return salt;
    }

    private static String toHex(byte[] bytes) {
        StringBuilder sb = new StringBuilder(bytes.length * 2);
        for (byte b : bytes) {
            sb.append(String.format("%02x", b));
        }
        return sb.toString();
    }

    private static byte[] fromHex(String hex) {
        int len = hex.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(hex.charAt(i), 16) << 4)
                    + Character.digit(hex.charAt(i + 1), 16));
        }
        return data;
    }
}
