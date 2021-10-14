using System;

namespace Sudoku.Solver.Application
{
    public static class MapGeneratorHelper
    {
        public static int[][] Transpose(int[][] matrix)
        {
            var transpose = new int[matrix.Length][];
            for (int i = 0; i < matrix.Length; i++)
            {
                transpose[i] = new int[matrix[i].Length];
                for (int j = 0; j < matrix[i].Length; j++)
                {
                    transpose[i][j] = matrix[j][i];
                }
            }

            return transpose;
        }

        public static bool IsSafeInRow(this Cell[] row, int randomNumber)
        {
            var hasSameItem = HasSameItem(row, randomNumber);
            if (hasSameItem)
            {
                return false;
            }

            var sum = Sum(row);
            return sum <= Constants.SumNumber;
        }

        public static bool IsSafeInColumn(this Cell[][] map, int colIndex, int randomNumber)
        {
            var col = new Cell[Constants.Size];
            for (int i = 0; i < map.Length; i++)
            {
                col[i] = new Cell(i, colIndex, map[i][colIndex].Value);
            }

            return IsSafeInRow(col, randomNumber);
        }

        public static bool HasSameItem(this Cell[] row, int number)
        {
            var hasSameItem = false;
            var counter = 0;
            while (!hasSameItem && counter < Constants.Size)
            {
                hasSameItem = row[counter].Value == number;
                counter++;
            }

            return hasSameItem;
        }

        public static int Sum(this Cell[] row)
        {
            var sum = 0;
            for (int i = 0; i < row.Length; i++)
            {
                sum += row[i].Value;
            }
            return sum;
        }

        //Function to get a random number 
        private static readonly Random _random = new Random();
        private static readonly object _syncLock = new object();
        public static int RandomNumber(int min, int max)
        {
            lock (_syncLock)
            { // synchronize
                return _random.Next(min, max);
            }
        }
    }
}
