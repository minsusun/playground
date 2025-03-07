package edu.postech.csed232.homework1;

/**
 * Given two non-negative integers, number and target, determine if target is equivalent to
 * an expression obtained by inserting '+' or '-' between the digits of number.
 * <p>
 * Hint: Use recursion to try all possible combinations of '+' and '-'.
 */
public class ExpressionAddOperator {
    /**
     * Determines if it is possible to insert '+' or '-' between the digits of 'number' to
     * obtain 'target'. Both 'number' and 'target' are non-negative integers. For example,
     * if number = 1234 and target = 20, the method should return true since 1 + 23 - 4 = 20.
     *
     * @param number a non-negative integer
     * @param target a non-negative integer
     * @return true if 'target' can be obtained from 'number'
     * @throws IllegalArgumentException if number or target is negative
     */
    public boolean solve(int number, int target) {
        //TODO: implement this method
        if (number < 0 || target < 0) {
            throw new IllegalArgumentException();
        }
        return search(number, target);
    }

    private boolean search(int number, int target) {
        // 재귀 탐색 함수
        if (number == target) {
            return true;
        }
        // 현재 수에서 끊을 수 있는 모든 경우로 나누어 탐색
        for (int i = 0, div = 10; i < Integer.toString(number).length() - 1; i++, div *= 10) {
            if (search(number / div, target + number % div)
                    || search(number / div, target - number % div)) {
                return true;
            }
        }
        return false;
    }

    public static void main(String[] args) {
        var number = 1234;
        var target = 20;
        var checker = new ExpressionAddOperator();

        System.out.println("Can " + number + " form " + target + "? " + checker.solve(number, target));
    }
}
