using System;
using System.Collections.Generic;
using System.Linq;

namespace Sudoku.Solver.Application
{
    public class MapGenerator
    {
        private int _row;
        private int _col;
        private int _stepCnt = 0;
        private readonly Dictionary<int, List<int>> _rowCandidates = new Dictionary<int, List<int>>();
        private readonly Dictionary<int, List<int>> _colCandidates = new Dictionary<int, List<int>>();
        private readonly Dictionary<int, List<int>> _boxCandidates = new Dictionary<int, List<int>>();

        public int StepCount 
        { 
            get 
            {
                return _stepCnt;
            }
        }

        public void Generate(Table table)
        {
            do
            {
                table.Init();
                Reset();

                Forward(table);
            }
            while (!IsValid(table));            
        }

        private void Reset()
        {
            _stepCnt = 0;
            _row = 0;
            _col = 0;

            for (int i = 0; i < Constants.Size; i++)
            {
                var list = new List<int>();
                for (int j = 1; j <= Constants.Size; j++)
                {
                    list.Add(j);
                }

                _rowCandidates[i] = list.ToList();
                _colCandidates[i] = list.ToList();

                _boxCandidates[i] = new List<int>();
            }
        }

        // TODO: Exceptions are NOT FUN.
        public bool IsValid(Table table)
        {
            try
            {
                for (int i = 0; i < Constants.Size; i++)
                {
                    for (var j = 0; j < Constants.Size; j++)
                    {
                        if (table.Map[i].Count(x => x.Value == table.Map[i][j].Value) != 1)
                        {
                            throw new Exception();
                        }                        
                    }
                }

                var transpose = MapGeneratorHelper.Transpose(table.GetValues());

                for (int i = 0; i < Constants.Size; i++)
                {
                    for (var j = 0; j < Constants.Size; j++)
                    {
                        if (transpose[i].Count(x => x == transpose[i][j]) != 1)
                        {
                            throw new Exception($"{j} {i}: {transpose[i][j]}");
                        }
                    }
                }

                return true;
            }
            catch(Exception ex)
            {
                Console.WriteLine($"{ex}");
                Program.WriteMap(table.GetValues());

                return false;
            }
            
        }

        public void Forward(Table table)
        {
            while (_row < Constants.Size)
            {
                _stepCnt++;

                if (_rowCandidates[_row].Count == 0)
                {
                    ResetRow(table);
                    continue;
                }

                if (!_rowCandidates[_row].Any(r => _colCandidates[_col].Any(c => c == r)))
                {
                    ResetRow(table);
                    continue;
                }

                if (!_rowCandidates[_row].Any(r => IsSafeInBox(r)))
                {
                    ResetRow(table);
                    continue;
                }

                var index = _rowCandidates[_row].Count == 1 ? 0 : -1;
                if (_rowCandidates[_row].Count > 1)
                {
                    var attemps = new List<int>();
                    do
                    {
                        index = MapGeneratorHelper.RandomNumber(0, _rowCandidates[_row].Count);

                        if (!attemps.Any(x => x == index))
                            attemps.Add(index);
                    }
                    // TODO: column check is not ok
                    while ((!IsSafeInBox(_rowCandidates[_row][index])
                        || !table.Map.IsSafeInColumn(_col, _rowCandidates[_row][index]))
                        && attemps.Count < _rowCandidates[_row].Count);
                }

                if (index < 0)
                {
                    ResetRow(table);
                    continue;
                }

                var candidate = _rowCandidates[_row][index];
                _rowCandidates[_row].Remove(candidate);
                _colCandidates[_col].Remove(candidate);

                AddBox(candidate);
                table.Map[_row][_col].Value = candidate;

                _col++;
                if (_col > (Constants.Size - 1))
                {
                    _row++;
                    _col = 0;
                }
            }

        }

        private void ResetRow(Table table)
        {
            for (int i = Constants.Size - 1; i >= 0; i--)
            {
                var candidate = table.Map[_row][i].Value;
                if (candidate > 0)
                {
                    _rowCandidates[_row].Add(candidate);
                    _colCandidates[i].Add(candidate);

                    var index = DetermineBoxIndex(i);

                    _boxCandidates[index].Remove(candidate);
                }

                table.Map[_row][i].Value = 0;
            }

            _row--;
            _col = 0;

            if (_row < 0)
            {
                _row = 0;
            }
        }

        private int DetermineBoxIndex(int col)
        {
            var rowIndex = _row == 0 ? 0 : (int)(_row / 3);
            var colIndex = col == 0 ? 0 : (int)(col / 3);

            var index = (rowIndex * 3) + colIndex;
            return index;
        }

        private int DetermineBoxIndex()
        {
            return DetermineBoxIndex(_col);
        }

        private bool IsSafeInBox(int number)
        {
            var index = DetermineBoxIndex();

            return !_boxCandidates[index].Any(x => x == number);
        }

        private void AddBox(int number)
        {
            var index = DetermineBoxIndex();

            _boxCandidates[index].Add(number);
        }

    }
}
