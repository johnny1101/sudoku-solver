namespace Sudoku.Solver.Application
{
    public class Table
    {
        public Cell[][] Map { get; set; } = new Cell[Constants.Size][];
        
        public int[][] GetValues()
        {
            var result = new int[Constants.Size][];
            for (int i = 0; i < Constants.Size; i++)
            {
                result[i] = new int[Constants.Size];
                for (int j = 0; j < Constants.Size; j++)
                {
                    result[i][j] = Map[i][j].Value;
                }
            }

            return result;
        }

        public void Init()
        {
            for (int i = 0; i < Constants.Size; i++)
            {
                Map[i] = new Cell[Constants.Size];
                for (int j = 0; j < Constants.Size; j++)
                {
                    Map[i][j] = new Cell(i, j);
                }
            }
        }
    }
}
