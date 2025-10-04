# reasoning_system

## Jak działa ten program

To zaawansowany system rozumowania AI, który symuluje podejmowanie decyzji przez inteligentny agent. Oto jak działa:

### 🧠 **Główne komponenty:**

**1. System Kontekstu**
- Przechowuje dane o środowisku (temperatura, bateria, wilgotność, czas)
- Dynamicznie aktualizuje się na podstawie zmian

**2. Świeże Wnioski (Fresh Inference)**
- Symuluje "emocje" i intuicje AI
- Mają wagę czasową - tracą na znaczeniu z czasem
- Wpływają na proces decyzyjny (np. "za gorąco dziś" z wagą -0.8)

**3. Zaawansowane Warunki (Advanced Conditions)**
- Każdy warunek ma:
  - Funkcję ewaluacji (czy jest spełniony)
  - Poziom pewności (0-1)
  - Modyfikator emocjonalny
  - Sprawdzenie istotności

**4. Detektor Wzorców**
- Uczy się z historii decyzji
- Przewiduje skutki na podstawie podobnych sytuacji
- Buduje bazę wiedzy "jeśli-to-wtedy"

**5. Analiza Zysków/Strat**
- Ocenia każdą akcję pod kątem:
  - Korzyści lokalnych (natychmiastowych)
  - Korzyści globalnych (długoterminowych)
  - Kosztów i ryzyka

**6. Resolver Konfliktów**
- Łączy wszystkie komponenty
- Podejmuje decyzje nawet przy sprzecznych warunkach
- Używa progów elastycznych (nie wymaga 100% pewności)

### 🎯 **Przykład działania:**

Program testuje decyzję "włącz klimatyzację":
- **Warunki:** temperatura > 25°C ✅, bateria > 50% ❌, godziny pracy ✅
- **Emocje:** "za gorąco dziś" (-0.8), "bateria stresuje" (-0.5)
- **Analiza:** komfort lokalny vs koszty energii
- **Wzorce:** czy podobne sytuacje kończyły się dobrze?
- **Decyzja:** TAK/NIE z uzasadnieniem i poziomem pewności

## 🚀 Jak rozwijać ten system

### **Krótkoterminowe ulepszenia:**

**1. Więcej typów warunków**
```cpp
// Warunki czasowe
AdvancedCondition("weekend", [](const sys&) { 
    return getCurrentDayOfWeek() >= 6; 
});

// Warunki prognozy
AdvancedCondition("pogoda_jutro", [](const sys&) {
    return getWeatherForecast() == "sunny";
});

// Warunki użytkownika
AdvancedCondition("preferencje_uzytkownika", [](const sys&) {
    return getUserPreference("comfort_priority") > 0.7;
});
```

**2. Rozszerzona analiza kosztów**
```cpp
// Dodaj więcej wymiarów
struct ExtendedAnalysis {
    double environmentalImpact;
    double userSatisfaction;
    double longTermConsequences;
    double socialImpact;
    std::vector<std::string> alternativeActions;
};
```

**3. Uczenie się z feedbacku**
```cpp
class FeedbackLearner {
    void recordOutcome(const std::string& action, 
                      double userSatisfaction,
                      double actualCost);
    void adjustWeights();
};
```

### **Średnioterminowe rozszerzenia:**

**4. Multi-agent system**
```cpp
class AgentCommunication {
    // Komunikacja między agentami
    void sharePattern(const Pattern& pattern);
    void requestAdvice(const std::string& situation);
    std::vector<Recommendation> getRecommendations();
};
```

**5. Hierarchiczne cele**
```cpp
class GoalHierarchy {
    std::vector<Goal> subgoals;
    std::map<std::string, double> goalDependencies;
    void optimizeGoalConflicts();
};
```

**6. Uczenie przez wzmocnienie**
```cpp
class ReinforcementLearner {
    double qTable[states][actions];
    void updateQValue(int state, int action, double reward);
    int selectAction(int state); // epsilon-greedy
};
```

### **Długoterminowe wizje:**

**7. Integracja z prawdziwym AI**
```cpp
class NeuralNetworkIntegration {
    // Połączenie z TensorFlow/PyTorch
    std::vector<double> predictOutcome(const Context& ctx);
    void trainOnHistoricalData();
};
```

**8. Rozumowanie przyczynowe**
```cpp
class CausalReasoning {
    // Graf przyczynowy
    std::map<std::string, std::vector<std::string>> causes;
    std::map<std::string, std::vector<std::string>> effects;
    
    bool wouldCause(const std::string& action, 
                   const std::string& outcome);
};
```

**9. Planowanie długoterminowe**
```cpp
class LongTermPlanner {
    std::vector<Action> createPlan(const Goal& goal, 
                                  int timeHorizon);
    void adaptPlanToChanges();
};
```

### **Praktyczne zastosowania:**

**Smart Home:** Zarządzanie klimatyzacją, oświetleniem, bezpieczeństwem
**Autonomiczne pojazdy:** Decyzje o trasie, prędkości, manewrach  
**Systemy biznesowe:** Optymalizacja zasobów, planowanie produkcji
**Gry AI:** Inteligentne NPC z realistycznym zachowaniem
**Systemy medyczne:** Wspomaganie diagnozy i leczenia

### **Następne kroki programistyczne:**

1. **Dodaj interfejs konfiguracyjny** - JSON/XML do definiowania warunków
2. **Zaimplementuj persistence** - zapisywanie wzorców do bazy danych
3. **Stwórz API** - RESTful interface dla zewnętrznych aplikacji
4. **Dodaj wizualizację** - graficzne przedstawienie procesu decyzyjnego
5. **Zoptymalizuj wydajność** - cache'owanie, równoległość

Ten system to solidna podstawa do budowy zaawansowanych AI agents - ma już kluczowe elementy: uczenie się, rozumowanie niepewne, rozwiązywanie konfliktów i adaptację.
