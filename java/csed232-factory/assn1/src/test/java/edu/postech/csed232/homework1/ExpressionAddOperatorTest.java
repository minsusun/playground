package edu.postech.csed232.homework1;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class ExpressionAddOperatorTest {

    final ExpressionAddOperator solver = new ExpressionAddOperator();

    @ParameterizedTest
    @CsvSource({
            "1234, 20, true",
            "1234, 22, false",
            "123, 0, true",
            "123, 7, false",
            "105, 5, true",
            "105, 7, false",
            "12345, 15, true",
            "12345, 100, false",
            "1111, 11, true",
            "1111, 5, false",
            "15, 17, false",
            "15, 15, true",
            "100200300, 100500, true",
            "100200300, 100000, false"
    })
    void testSolve(int number, int target, boolean expected) {
        assertEquals(expected, solver.solve(number, target));
    }
}
