package edu.postech.csed232.homework1;

/**
 * A number translator that converts numbers to words in English.
 */
@SuppressWarnings("GrazieInspection")
public class EnglishNumberTranslator {

    /**
     * Convert a number to words in English. The number should be in the range
     * [0, Long.MAX_VALUE], and otherwise an exception should be thrown. For example,
     * 1234567890 should be translated to "one billion two hundred thirty four million
     * five hundred sixty seven thousand eight hundred ninety".
     *
     * @param number a number
     * @return a string of words
     * @throws IllegalArgumentException if the number is not in the range
     */
    public String toWords(long number) {
        //TODO: implement this method
        return "";
    }


    public static void main(String[] args) {
        var input = 10000001201L;
        var translator = new EnglishNumberTranslator();

        System.out.println(input + " -> " + translator.toWords(input));
    }

}
