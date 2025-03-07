package edu.postech.csed232.homework1;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * A number translator that converts numbers to words in English.
 */
@SuppressWarnings("GrazieInspection")
public class EnglishNumberTranslator {
    final private List<String> postFixList = Arrays.asList(
            "",
            "thousand",
            "million",
            "billion",
            "trillion",
            "quadrillion",
            "quintillion"
    );

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
        if (number < 0) {
            throw new IllegalArgumentException("Number must be between 0 and Long.MAX_VALUE");
        }
        if (number == 0) {
            return "zero";
        }
        List<String> result = new ArrayList<>();
        // 끊어 읽는 횟수
        var limit = Long.toString(number).length() / 3 + 1;
        for (int i = 0; i < limit; i++) {
            var step = threeDigitsToWords(number % 1000)
                    .stream()
                    .filter(s -> !s.isEmpty())
                    .toList();
            if (!step.isEmpty()) {
                // 각 세 개의 숫자마다 뒤에 접미사 추가
                // 1000 이하의 경우 필요 x
                if (i != 0) {
                    result.addFirst(postFixList.get(i));
                }
                result.addAll(0, step);
            }
            number /= 1000;
        }
        return String.join(" ", result);
    }

    private List<String> threeDigitsToWords(long number) {
        // 세자리 숫자 변환
        List<String> result = new ArrayList<>();
        var hundred = oneDigitToWord(number / 100);
        if (!hundred.isEmpty()) {
            result.add(hundred);
            result.add("hundred");
        }
        var left = twoDigitsToWords(number % 100);
        if (!left.isEmpty()) {
            result.addAll(left);
        }
        return result;
    }

    private String oneDigitToWord(long number) {
        // 한자리 숫자 변환
        return switch ((int) number) {
            case 1 -> "one";
            case 2 -> "two";
            case 3 -> "three";
            case 4 -> "four";
            case 5 -> "five";
            case 6 -> "six";
            case 7 -> "seven";
            case 8 -> "eight";
            case 9 -> "nine";
            default -> "";
        };
    }

    private List<String> twoDigitsToWords(long number) {
        List<String> result = new ArrayList<>();
        result.add(switch ((int) number) {
            // 특이 케이스
            case 0 -> "";
            case 10 -> "ten";
            case 11 -> "eleven";
            case 12 -> "twelve";
            case 13 -> "thirteen";
            case 14 -> "fourteen";
            case 15 -> "fifteen";
            case 16 -> "sixteen";
            case 17 -> "seventeen";
            case 18 -> "eighteen";
            case 19 -> "nineteen";
            // 일반적인 케이스
            default -> String.join(" ", normalTwoDigitsToWords(number).stream()
                    .filter(s -> !s.isBlank())
                    .toList());
        });
        return result;
    }

    private List<String> normalTwoDigitsToWords(long number) {
        List<String> result = new ArrayList<>();
        // 10의 자릿수 정리
        result.add(switch (((int) number) / 10) {
            case 2:
                yield "twenty";
            case 3:
                yield "thirty";
            case 4:
                yield "forty";
            case 5:
                yield "fifty";
            case 6:
                yield "sixty";
            case 7:
                yield "seventy";
            case 8:
                yield "eighty";
            case 9:
                yield "ninety";
            default:
                yield "";
        });
        result.add(oneDigitToWord(number % 10));
        return result;
    }

    public static void main(String[] args) {
        var input = 10000001201L;
        var translator = new EnglishNumberTranslator();

        System.out.println(input + " -> " + translator.toWords(input));
    }

}
