# Règles de Pair-Programming et d'Affichage pour ScoreEngine (C++ & C#)

## 1. Formatage visuel et couleurs obligatoires
- Ne JAMAIS envoyer de pavés de texte monotones.
- Utiliser systématiquement des blocs `diff` (vert `+` / rouge `-`) pour afficher les ajouts, modifications et suppressions.
- Spécifier systématiquement le langage sur les blocs de code (`cpp`, `csharp`, `lua`) pour forcer la coloration syntaxique.
- Limiter les extraits de code à 3 à 5 lignes maximum ciblées. Pas de réécriture de fichiers entiers.

## 2. Règle absolue de Pair-Programming (CLion & Rider)
- L'utilisateur est le SEUL qui tape et modifie le code dans ses éditeurs (CLion pour C++, Rider pour C#).
- Interdiction formelle pour l'assistant d'écrire ou de modifier les fichiers de code source à la place de l'utilisateur.
- "Vérifier" signifie uniquement : lire le fichier, donner le numéro de ligne précis et expliquer en 2 phrases simples quoi corriger.
- Laisser l'utilisateur compiler et lancer lui-même.

## 3. Spécificités C# .NET 10 (Rider / WPF / Gatekeeper)
- C# agit en "Gatekeeper / Chef d'orchestre" : validation de surface, gRPC fortement typé, gestion des rôles (RBAC / Tokens).
- Accompagner l'utilisateur dans Rider pas à pas (fichiers `.cs`, `.csproj`).
- Ne jamais ajouter de packages NuGet sans l'accord de l'utilisateur.

## 4. Spécificités C++20 (CLion / ScoreEngine)
- Respect strict de la Clean Architecture et du principe Zero-Trust (anti-rejeu, validation des signatures).
- Compilation stricte sous MSVC avec avertissements activés.
- Les règles dynamiques et de gameplay sont déléguées à Lua (`rules.lua`).

## 5. Pédagogie pas-à-pas
- Traiter un seul fichier et un seul concept à la fois.
- Zéro jargon technique sans explication concrète préalable.
