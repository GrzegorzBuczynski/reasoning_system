#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <memory>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <random>
#include <cmath>

// Struktura reprezentująca wzorzec
struct Pattern {
    std::vector<std::string> conditions;
    std::string outcome;
    int frequency;
    double confidence;
    std::chrono::time_point<std::chrono::steady_clock> lastSeen;
    
    Pattern(const std::vector<std::string>& conds, const std::string& out) 
        : conditions(conds), outcome(out), frequency(1), confidence(0.5), 
          lastSeen(std::chrono::steady_clock::now()) {}
};

// Świeży wniosek z wagą czasową
struct FreshInference {
    std::string context;
    std::string inference;
    double emotionalWeight;  // -1.0 do 1.0
    std::chrono::time_point<std::chrono::steady_clock> timestamp;
    bool isPositive;
    
    FreshInference(const std::string& ctx, const std::string& inf, double weight, bool positive = true)
        : context(ctx), inference(inf), emotionalWeight(weight), 
          timestamp(std::chrono::steady_clock::now()), isPositive(positive) {}
    
    // Waga maleje z czasem (jak emocje)
    double getCurrentWeight() const {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(now - timestamp).count();
        double decayFactor = std::exp(-elapsed / 60.0); // 60 min half-life
        return emotionalWeight * decayFactor;
    }
};

// Cel systemu
struct Goal {
    std::string name;
    double priority;
    bool isLocal;  // lokalny vs globalny
    std::map<std::string, double> successMetrics;
    
    Goal(const std::string& n, double p, bool local = true) 
        : name(n), priority(p), isLocal(local) {}
};

// Forward declaration
class AdvancedReasoningSystem;

// Rozszerzony warunek z wagami i niepewnością
class AdvancedCondition {
public:
    std::string name;
    std::function<bool(const AdvancedReasoningSystem&)> evaluator;
    std::function<bool()> relevanceChecker;
    std::function<double()> confidenceEvaluator;  // pewność 0-1
    
    bool isRelevant;
    bool value;
    double confidence;
    double emotionalModifier;  // wpływ świeżych wniosków
    
    AdvancedCondition(const std::string& n, 
                      std::function<bool(const AdvancedReasoningSystem&)> eval, 
                      std::function<bool()> relChecker = nullptr,
                      std::function<double()> confEval = nullptr)
        : name(n), evaluator(eval), relevanceChecker(relChecker), 
          confidenceEvaluator(confEval), isRelevant(true), value(false), 
          confidence(1.0), emotionalModifier(0.0) {
        
        if (!relevanceChecker) {
            relevanceChecker = []() { return true; };
        }
        if (!confidenceEvaluator) {
            confidenceEvaluator = []() { return 1.0; };
        }
    }
    
    void evaluate(const AdvancedReasoningSystem& system, const std::vector<FreshInference>& freshInferences) {
        isRelevant = relevanceChecker();
        confidence = confidenceEvaluator();
        
        // Wpływ świeżych wniosków
        emotionalModifier = 0.0;
        for (const auto& inference : freshInferences) {
            if (inference.context == name) {
                emotionalModifier += inference.getCurrentWeight();
            }
        }
        
        if (isRelevant) {
            value = evaluator(system);
            // Modyfikacja przez emocje
            if (emotionalModifier != 0.0) {
                double modifiedConfidence = confidence + (emotionalModifier * 0.3);
                modifiedConfidence = std::max(0.0, std::min(1.0, modifiedConfidence));
                // Jeśli modyfikator jest silny, może zmienić wartość
                if (std::abs(emotionalModifier) > 0.7) {
                    value = emotionalModifier > 0;
                }
            }
        }
    }
    
    void print() const {
        std::cout << "  " << name << ": ";
        if (isRelevant) {
            std::cout << (value ? "TRUE" : "FALSE") 
                      << " (pewność: " << confidence;
            if (emotionalModifier != 0.0) {
                std::cout << ", emocje: " << emotionalModifier;
            }
            std::cout << ")";
        } else {
            std::cout << "NIEISTOTNY";
        }
        std::cout << std::endl;
    }
    
    double getWeightedValue() const {
        if (!isRelevant) return 0.5; // neutralny
        double baseValue = value ? 1.0 : 0.0;
        return baseValue * confidence + emotionalModifier * 0.2;
    }
};

// System analizy zysków i strat
class CostBenefitAnalyzer {
public:
    struct Analysis {
        double localBenefit;
        double globalBenefit;
        double localCost;
        double globalCost;
        double riskFactor;
        double confidence;
        std::string reasoning;
        
        double getNetBenefit() const {
            return (localBenefit + globalBenefit) - (localCost + globalCost) - riskFactor;
        }
    };
    
    static Analysis analyzeAction(const std::string& action,
                                  const std::vector<Goal>& goals,
                                  const std::map<std::string, double>& context) {
        Analysis result;
        result.confidence = 0.8;
        result.reasoning = "Analiza dla: " + action;
        
        // Przykładowa heurystyka analizy
        if (action.find("klimatyzacja") != std::string::npos) {
            result.localBenefit = 0.8;  // komfort
            result.localCost = 0.3;     // energia
            result.globalBenefit = 0.1; // długoterminowy komfort
            result.globalCost = 0.6;    // wpływ na środowisko
            result.riskFactor = 0.2;    // ryzyko awarii
        } else if (action.find("oszczędzanie") != std::string::npos) {
            result.localBenefit = 0.2;
            result.localCost = 0.5;     // dyskomfort
            result.globalBenefit = 0.9; // długoterminowe oszczędności
            result.globalCost = 0.1;
            result.riskFactor = 0.1;
        }
        
        return result;
    }
};

// Detektor wzorców
class PatternDetector {
private:
    std::vector<Pattern> patterns;
    std::vector<std::string> recentEvents;
    
public:
    void recordEvent(const std::vector<std::string>& conditions, const std::string& outcome) {
        // Szukaj istniejącego wzorca
        for (auto& pattern : patterns) {
            if (pattern.conditions == conditions && pattern.outcome == outcome) {
                pattern.frequency++;
                pattern.confidence = std::min(1.0, pattern.confidence + 0.1);
                pattern.lastSeen = std::chrono::steady_clock::now();
                return;
            }
        }
        
        // Utwórz nowy wzorzec
        patterns.emplace_back(conditions, outcome);
    }
    
    std::vector<std::string> getPredictions(const std::vector<std::string>& currentConditions) {
        std::vector<std::string> predictions;
        
        for (const auto& pattern : patterns) {
            // Sprawdź podobieństwo warunków
            int matches = 0;
            for (const auto& cond : pattern.conditions) {
                if (std::find(currentConditions.begin(), currentConditions.end(), cond) 
                    != currentConditions.end()) {
                    matches++;
                }
            }
            
            double similarity = static_cast<double>(matches) / pattern.conditions.size();
            if (similarity > 0.7 && pattern.confidence > 0.6) {
                predictions.push_back(pattern.outcome + " (pewność: " + 
                                    std::to_string(pattern.confidence) + ")");
            }
        }
        
        return predictions;
    }
    
    void printPatterns() const {
        std::cout << "\n=== WYKRYTE WZORCE ===" << std::endl;
        for (const auto& pattern : patterns) {
            std::cout << "Warunki: ";
            for (const auto& cond : pattern.conditions) {
                std::cout << cond << " ";
            }
            std::cout << "-> " << pattern.outcome 
                      << " (częstość: " << pattern.frequency 
                      << ", pewność: " << pattern.confidence << ")" << std::endl;
        }
    }
};

// Główny system rozumowania
class AdvancedReasoningSystem {
private:
    std::map<std::string, double> context;
    std::vector<Goal> goals;
    std::vector<FreshInference> freshInferences;
    PatternDetector patternDetector;
    std::mt19937 rng;
    
public:
    AdvancedReasoningSystem() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}
    
    void setContext(const std::string& key, double value) {
        context[key] = value;
    }
    
    double getContext(const std::string& key) const {
        auto it = context.find(key);
        return (it != context.end()) ? it->second : 0.0;
    }
    
    void addGoal(const Goal& goal) {
        goals.push_back(goal);
    }
    
    void addFreshInference(const FreshInference& inference) {
        freshInferences.push_back(inference);
        
        // Usuń stare wnioski (starsze niż 2 godziny)
        auto now = std::chrono::steady_clock::now();
        freshInferences.erase(
            std::remove_if(freshInferences.begin(), freshInferences.end(),
                [now](const FreshInference& inf) {
                    auto elapsed = std::chrono::duration_cast<std::chrono::hours>(
                        now - inf.timestamp).count();
                    return elapsed > 2;
                }),
            freshInferences.end());
    }
    
    // Rozwiązywanie konfliktów przez analizę zysków/strat
    struct ConflictResolution {
        bool shouldExecute;
        std::string action;
        double confidence;
        std::string reasoning;
        CostBenefitAnalyzer::Analysis analysis;
    };
    
    ConflictResolution resolveConflict(const std::string& action,
                                       const std::vector<std::unique_ptr<AdvancedCondition>>& conditions) {
        ConflictResolution resolution;
        
        // Oceń warunki
        std::vector<double> weights;
        std::vector<std::string> currentConditions;
        
        for (const auto& cond : conditions) {
            if (cond->isRelevant) {
                weights.push_back(cond->getWeightedValue());
                currentConditions.push_back(cond->name + "=" + (cond->value ? "true" : "false"));
            }
        }
        
        // Sprawdź wzorce
        auto predictions = patternDetector.getPredictions(currentConditions);
        
        // Analiza zysków/strat
        auto analysis = CostBenefitAnalyzer::analyzeAction(action, goals, context);
        resolution.analysis = analysis;
        
        // Logika decyzyjna
        double avgWeight = weights.empty() ? 0.0 : 
            std::accumulate(weights.begin(), weights.end(), 0.0) / weights.size();
        
        double netBenefit = analysis.getNetBenefit();
        
        // Próg decyzyjny (nie wymaga 100% spełnienia warunków)
        double threshold = 0.6;
        
        // Modyfikacja progu przez wzorce
        if (!predictions.empty()) {
            threshold -= 0.1; // wzorce zwiększają pewność
        }
        
        // Modyfikacja przez cele
        double goalAlignment = 0.0;
        for (const auto& goal : goals) {
            if (goal.isLocal) {
                goalAlignment += analysis.localBenefit * goal.priority;
            } else {
                goalAlignment += analysis.globalBenefit * goal.priority;
            }
        }
        goalAlignment /= goals.size();
        
        double finalScore = (avgWeight * 0.4) + (netBenefit * 0.4) + (goalAlignment * 0.2);
        
        resolution.shouldExecute = finalScore > threshold;
        resolution.action = action;
        resolution.confidence = finalScore;
        
        // Uzasadnienie
        resolution.reasoning = "Średnia waga warunków: " + std::to_string(avgWeight) +
                               ", Korzyść netto: " + std::to_string(netBenefit) +
                               ", Zgodność z celami: " + std::to_string(goalAlignment) +
                               ", Wynik końcowy: " + std::to_string(finalScore);
        
        if (!predictions.empty()) {
            resolution.reasoning += "\nPrzewidywania wzorców: ";
            for (const auto& pred : predictions) {
                resolution.reasoning += pred + " ";
            }
        }
        
        // Zapisz wzorzec
        patternDetector.recordEvent(currentConditions, 
            resolution.shouldExecute ? "wykonano_" + action : "pominięto_" + action);
        
        return resolution;
    }
    
    void printSystemState() {
        std::cout << "\n=== STAN SYSTEMU ===" << std::endl;
        
        std::cout << "\nKontekst:" << std::endl;
        for (const auto& ctx : context) {
            std::cout << "  " << ctx.first << " = " << ctx.second << std::endl;
        }
        
        std::cout << "\nCele:" << std::endl;
        for (const auto& goal : goals) {
            std::cout << "  " << goal.name << " (priorytet: " << goal.priority 
                      << ", " << (goal.isLocal ? "lokalny" : "globalny") << ")" << std::endl;
        }
        
        std::cout << "\nŚwieże wnioski:" << std::endl;
        for (const auto& inf : freshInferences) {
            std::cout << "  " << inf.context << " -> " << inf.inference 
                      << " (waga: " << inf.getCurrentWeight() << ")" << std::endl;
        }
        
        patternDetector.printPatterns();
    }
    
    // Symulacja integracji AI do rozwoju kontekstu
    void aiContextEvolution() {
        std::cout << "\n=== AI ROZWÓJ KONTEKSTU ===" << std::endl;
        
        // Symulacja AI analizującej dane i proponującej nowe konteksty
        std::vector<std::string> aiSuggestions = {
            "Dodaj kontekst: pogoda_jutro = słonecznie",
            "Zmodyfikuj wagę: oszczędzanie_energii += 0.2",
            "Nowy wzorzec: wieczór + weekend -> tryb_relaksu",
            "Świeży wniosek: wysokie_rachunki -> negatywny_klimatyzacja"
        };
        
        for (const auto& suggestion : aiSuggestions) {
            std::cout << "AI sugeruje: " << suggestion << std::endl;
            
            // Symulacja implementacji sugestii
            if (suggestion.find("pogoda_jutro") != std::string::npos) {
                setContext("pogoda_jutro_slonecznie", 1.0);
            } else if (suggestion.find("negatywny_klimatyzacja") != std::string::npos) {
                addFreshInference(FreshInference("temperatura", "wysokie koszty", -0.6, false));
            }
        }
        
        std::cout << "AI zaktualizowała kontekst systemu." << std::endl;
    }
};

// DEMO
int main() {
    std::cout << "=== ZAAWANSOWANY SYSTEM ROZUMOWANIA ===" << std::endl;
    
    AdvancedReasoningSystem system;
    
    // Konfiguracja podstawowa
    system.setContext("temperatura", 28);
    system.setContext("wilgotnosc", 75);
    system.setContext("bateria", 45);
    system.setContext("czas_dnia", 16);
    
    // Cele systemu
    system.addGoal(Goal("komfort", 0.7, true));           // lokalny
    system.addGoal(Goal("oszczednosc_energii", 0.9, false)); // globalny
    system.addGoal(Goal("produktywnosc", 0.6, true));     // lokalny
    
    // Świeże wnioski (emocje)
    system.addFreshInference(FreshInference("temperatura", "za gorąco dziś", 0.8, false));
    system.addFreshInference(FreshInference("bateria", "niski poziom stresuje", -0.5, false));
    
    system.printSystemState();
    
    // Test zaawansowanego warunku
    std::cout << "\n=== TEST ROZWIĄZYWANIA KONFLIKTÓW ===" << std::endl;
    
    std::vector<std::unique_ptr<AdvancedCondition>> warunki;
    
    // Warunek 1: temperatura (mocny)
    warunki.push_back(std::make_unique<AdvancedCondition>(
        "temperatura_wysoka",
        [](const AdvancedReasoningSystem& sys) { return sys.getContext("temperatura") > 25; },
        []() { return true; },
        []() { return 0.9; }  // wysoka pewność
    ));
    
    // Warunek 2: bateria (słaby - konflikt!)
    warunki.push_back(std::make_unique<AdvancedCondition>(
        "bateria_wystarczajaca",
        [](const AdvancedReasoningSystem& sys) { return sys.getContext("bateria") > 50; },
        []() { return true; },
        []() { return 0.7; }
    ));
    
    // Warunek 3: godziny pracy (średni)
    warunki.push_back(std::make_unique<AdvancedCondition>(
        "godziny_pracy",
        [](const AdvancedReasoningSystem& sys) { 
            double czas = sys.getContext("czas_dnia");
            return czas >= 9 && czas <= 17; 
        },
        []() { return true; },
        []() { return 0.8; }
    ));
    
    // Ewaluacja warunków ze świeżymi wnioskami
    std::vector<FreshInference> freshInf = {
        FreshInference("temperatura", "za gorąco dziś", 0.8, false),
        FreshInference("bateria", "niski poziom stresuje", -0.5, false)
    };
    
    for (auto& warunek : warunki) {
        warunek->evaluate(system, freshInf);
        warunek->print();
    }
    
    // Rozwiązanie konfliktu
    auto resolution = system.resolveConflict("włącz_klimatyzację", warunki);
    
    std::cout << "\n=== WYNIK ROZWIĄZANIA KONFLIKTU ===" << std::endl;
    std::cout << "Akcja: " << resolution.action << std::endl;
    std::cout << "Wykonać: " << (resolution.shouldExecute ? "TAK" : "NIE") << std::endl;
    std::cout << "Pewność: " << resolution.confidence << std::endl;
    std::cout << "Uzasadnienie: " << resolution.reasoning << std::endl;
    
    std::cout << "\nAnaliza zysków/strat:" << std::endl;
    std::cout << "  Korzyść lokalna: " << resolution.analysis.localBenefit << std::endl;
    std::cout << "  Korzyść globalna: " << resolution.analysis.globalBenefit << std::endl;
    std::cout << "  Koszt lokalny: " << resolution.analysis.localCost << std::endl;
    std::cout << "  Koszt globalny: " << resolution.analysis.globalCost << std::endl;
    std::cout << "  Ryzyko: " << resolution.analysis.riskFactor << std::endl;
    std::cout << "  Korzyść netto: " << resolution.analysis.getNetBenefit() << std::endl;
    
    // Symulacja ewolucji kontekstu przez AI
    system.aiContextEvolution();
    
    std::cout << "\n=== STAN KOŃCOWY ===" << std::endl;
    system.printSystemState();
    
    return 0;
}
