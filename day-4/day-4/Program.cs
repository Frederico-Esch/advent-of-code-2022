struct Pair
{
    private int begin;
    private int end;

    public Pair(string text)
    {
        var numbers = text.Split("-").ToList().ConvertAll(Convert.ToInt32);
        begin = numbers[0];
        end = numbers[1];
    }

    public override string ToString()
    {
        return $"{begin}-{end}";
    }

    public bool Contain(Pair pair)
    {
        if (begin == pair.begin || end == pair.end) return true;

        if (begin < pair.begin)
            return end > pair.end;
        else
            return pair.end > end;
    }

    public bool Overlap(Pair pair)
    {
        return  !(end < pair.begin || begin > pair.end);
    }
}

internal static class Program
{
    private static void FirstChallenge()
    {
        int score = 0;

        using var filestream = File.Open("../../../input", FileMode.Open);
        using var buffer = new StreamReader(filestream);
        while (!buffer.EndOfStream)
        {
            var line = buffer.ReadLine()?.Trim() ?? "";
            if (line == "") break;
            var pairStrings = Separate(line);

            var contains = pairStrings.Item1.Contain(pairStrings.Item2);
            Console.WriteLine($"{pairStrings.Item1} - {pairStrings.Item2} - {contains}");
            if (contains)
                score += 1;
        }

        Console.WriteLine($"AMOUNT: {score}");
    }

    private static void SecondChallenge()
    {
        int score = 0;

        using var filestream = File.Open("../../../input", FileMode.Open);
        using var buffer = new StreamReader(filestream);
        while (!buffer.EndOfStream)
        {
            var line = buffer.ReadLine()?.Trim() ?? "";
            if (line == "") break;
            var pairStrings = Separate(line);

            var contains = pairStrings.Item1.Overlap(pairStrings.Item2);
            Console.WriteLine($"{pairStrings.Item1} - {pairStrings.Item2} - {contains}");
            if (contains)
                score += 1;
        }

        Console.WriteLine($"AMOUNT: {score}");
    }

    private static Tuple<Pair, Pair> Separate(string line)
    {
        var pairsStrings = line.Split(",");
        var tuple = new Tuple<Pair, Pair>(new Pair(pairsStrings[0]), new Pair(pairsStrings[1]));
        return tuple;
    }

    public static void Main()
    {
        SecondChallenge();
    }
}