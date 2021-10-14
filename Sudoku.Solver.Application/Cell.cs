using System.Collections.Generic;

namespace Sudoku.Solver.Application
{
    public class Cell
    {
        public List<int> Candidates { get; } = new List<int>();        
        public int Row { get; }
        public int Col { get; }
        public int Value { get; set; }

        public Cell(int row, int col)
        {
            Row = row;
            Col = col;
        }

        public Cell(int row, int col, int value): this(row, col)
        {
            Value = value;
        }

        public bool HasCandidate(int candidate)
        {
            return Candidates.Contains(candidate);
        }

        public void AddCandidate(int candidate)
        {
            Candidates.Add(candidate);
        }

    }
}
