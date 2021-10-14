using System;
using System.Text;

namespace Sudoku.Solver.Application
{

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Sudoku Resolver");
            Console.WriteLine("~~~~~~~~~~~~~~~");
            Console.WriteLine();

            var table = new Table();

            var mapGenerator = new MapGenerator();
            mapGenerator.Generate(table);

            Console.WriteLine($"Generáló lépések száma: {mapGenerator.StepCount}");

            Program.WriteMap(table.GetValues());

            //var solver = new Solver();
            //solver.Solve(table);

            //Program.WriteMap(table.GetValues());

        }

        public static void WriteMap(int[][] map)
        {
            var strBuilder = new StringBuilder();

            WriteHorizontalLine();
            for (var i = 0; i < Constants.Size; i++)
            {
                BuildRow(map[i], strBuilder);
                Console.WriteLine(strBuilder);

                if ((i + 1) % 3 == 0)
                {
                    WriteHorizontalLine();
                }

                strBuilder.Clear();
            }
        }

        private static void BuildRow(int[] row, StringBuilder strBuilder)
        {
            strBuilder.Append("| ");

            for (var i = 0; i < row.Length; i++)
            {
                strBuilder.Append($"{row[i]} ");

                if ((i + 1) % 3 == 0)
                {
                    strBuilder.Append("| ");
                }
            }
        }

        private static void WriteHorizontalLine()
        {
            for (var i = 0; i < (2 * Constants.Size) + 7; i++)
            {
                Console.Write('-');
            }
            Console.WriteLine();
        }
    }
}
