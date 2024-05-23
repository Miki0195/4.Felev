using System;
using System.Collections.Generic;
using System.Threading;

class Program
{
    static void Main(string[] args)
    {
        List<Question> questions = new List<Question>
        {
            new Question("Melyik nem agilis szoftverfejlesztési módszertan szerinti modell az alábbiak közül?",
                new List<string> { "Scrum", "Rational Unified Process (RUP)", "Extreme Programming (XP)", "Kanban" }, 2),
            new Question("Melyik nem része a rendszertervnek?",
                new List<string> { "Költségbecslés", "Kockázatelemzés", "Adatbázisterv", "Rendszerarchitektúra" }, 1),
            new Question("Melyik nem funkciója a projektmenedzsment eszközöknek?",
                new List<string> { "Időtervezés és ütemezés", "Erőforrás-kezelés", "Költségkövetés", "UML diagramok elkészítése és elhelyezése a tervben (case tooling)." }, 4)
            // Add more questions as needed
        };

        int score = 0;

        foreach (var question in questions)
        {
            Console.Clear();
            Console.WriteLine(question.Text);
            for (int i = 0; i < question.Answers.Count; i++)
            {
                Console.WriteLine($"{i + 1}. {question.Answers[i]}");
            }

            int userAnswer = GetAnswerFromUser(question.Answers.Count);
            if (userAnswer == question.CorrectAnswer)
            {
                Console.WriteLine("Helyes válasz!");
                score++;
            }
            else
            {
                Console.WriteLine($"Nem helyes válasz. A helyes válasz: {question.CorrectAnswer}. {question.Answers[question.CorrectAnswer - 1]}");
            }

            Thread.Sleep(2000); // 2 másodperc késleltetés
        }

        Console.WriteLine($"Az elért pontszámod: {score}/{questions.Count}");
        Console.ReadKey();
    }

    static int GetAnswerFromUser(int numberOfChoices)
    {
        int userAnswer = 0;
        bool validAnswer = false;
        while (!validAnswer)
        {
            Console.Write("A válaszod (1-{0}): ", numberOfChoices);
            string input = Console.ReadLine();
            if (int.TryParse(input, out userAnswer) && userAnswer >= 1 && userAnswer <= numberOfChoices)
            {
                validAnswer = true;
            }
            else
            {
                Console.WriteLine("Érvénytelen válasz. Kérlek, adj meg egy számot 1 és {0} között.", numberOfChoices);
            }
        }
        return userAnswer;
    }
}

class Question
{
    public string Text { get; }
    public List<string> Answers { get; }
    public int CorrectAnswer { get; }

    public Question(string text, List<string> answers, int correctAnswer)
    {
        Text = text;
        Answers = answers;
        CorrectAnswer = correctAnswer;
    }
}
