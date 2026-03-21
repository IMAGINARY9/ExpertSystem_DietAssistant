# ExpertSystem_DietAssistant

## Overview
ExpertSystem_DietAssistant is a Qt desktop expert system that helps users identify likely nutrient deficiencies from symptoms and then suggests foods that can help.

The application starts with a short questionnaire, then narrows possible nutrient gaps based on selected symptoms.

## Why This Project Matters
- Demonstrates a practical expert-system workflow for health education
- Connects symptoms to nutrients and nutrients to food suggestions
- Keeps domain data in editable text resources rather than hardcoded values

## Quick Start
1. Open `ExpertSystem_DietAssistant/ExpertSystemV2/ExpertSystemV2.pro` in Qt Creator.
2. Build with a Qt desktop kit that supports Widgets and C++17.
3. Run the application and complete the initial checklist dialog.
4. Select or remove symptoms to refine results, then click Finish.

## Features
- Initial checklist dialog for high-level concerns
- Searchable symptom list
- Category filter for symptom groups
- Selected-symptom panel with Select, Remove, and Clear actions
- Final recommendation dialog with missing nutrients and suggested foods

## Technology Stack
- Language: C++17
- Framework: Qt (Core, Gui, Widgets)
- Project format: qmake (`.pro`)
- UI: Qt Widgets (`mainwindow.ui`, `questionsdialog.ui`)

## Prerequisites
### System Requirements
- Qt 5 or newer with Widgets module
- C++17-compatible compiler
- qmake (or Qt Creator with qmake support)

### Dependencies
- No external runtime services
- Data is loaded from embedded Qt resource files

## Build Instructions
### Option 1: Qt Creator (recommended)
1. Open `ExpertSystem_DietAssistant/ExpertSystemV2/ExpertSystemV2.pro`.
2. Select a valid desktop Qt kit.
3. Build and run.

### Option 2: Command line with qmake
From `ExpertSystem_DietAssistant/ExpertSystemV2/`:

```bash
qmake ExpertSystemV2.pro
make
```

On Windows with MinGW, `mingw32-make` may be required instead of `make`.

## Running the Application
1. Launch the application.
2. In the question dialog, select applicable statements and confirm.
3. Use search and categories to find symptoms.
4. Move symptoms into the selected list.
5. Click Finish to receive nutrient and food recommendations.

If the system cannot determine a narrow enough result, it asks for more selected symptoms.

## Project Structure
- `ExpertSystem_DietAssistant/ExpertSystemV2/main.cpp`: application entry point
- `ExpertSystem_DietAssistant/ExpertSystemV2/expertsystem.cpp`: core expert-system logic
- `ExpertSystem_DietAssistant/ExpertSystemV2/mainwindow.cpp`: main UI interactions
- `ExpertSystem_DietAssistant/ExpertSystemV2/questionsdialog.cpp`: startup questionnaire dialog
- `ExpertSystem_DietAssistant/ExpertSystemV2/nutries&symptoms.txt`: symptom mapping data
- `ExpertSystem_DietAssistant/ExpertSystemV2/nutries&products.txt`: nutrient-to-food recommendations
- `ExpertSystem_DietAssistant/ExpertSystemV2/nutries&questions.txt`: startup question mapping
- `ExpertSystem_DietAssistant/ExpertSystemV2/sortCategories.txt`: symptom category grouping

## Data Updates
To expand or adjust recommendations, edit the text files in `ExpertSystem_DietAssistant/ExpertSystemV2/` and preserve the existing line format (`key: value1, value2, ...`).

## Known Limitations
- Recommendations are rule-based and do not replace medical advice
- Matching relies on exact symptom text in the provided datasets
- No persistent user profile between sessions

## License
MIT License. See `ExpertSystem_DietAssistant/LICENSE`.