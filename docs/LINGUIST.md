# Adding Lex to GitHub Linguist

GitHub uses [linguist](https://github.com/github-linguist/linguist) to detect programming languages. To have "Lex" appear as a recognized language, we need to contribute to the linguist project.

## Requirements

Before submitting a PR to linguist, Lex must be used in **200+ public repositories** on GitHub.

## Files to Prepare

When we reach the threshold, we'll need to submit these files:

### 1. Language Definition (`lib/linguist/languages.yml`)

```yaml
Lex:
  type: programming
  color: "#4F5D95"  # A nice blue-purple
  extensions:
    - ".lex"
  tm_scope: source.lex
  ace_mode: text
  language_id: 123456  # Assigned by linguist maintainers
```

### 2. Grammar File (`grammars/lex.cson`)

Already prepared in `editors/vscode/syntaxes/lex.tmLanguage.json`

### 3. Sample Files (`samples/Lex/`)

Include representative `.lex` files showing the language features.

## Process

1. Fork https://github.com/github-linguist/linguist
2. Add language definition to `lib/linguist/languages.yml`
3. Add grammar to `vendor/grammars/`
4. Add samples to `samples/Lex/`
5. Run tests: `bundle exec rake test`
6. Create PR with:
   - Clear description of the language
   - Link to this repository
   - Link to grammar/source

## Current Status

- [ ] 200+ repositories using `.lex` files
- [ ] Language definition prepared
- [x] Grammar file prepared (`editors/vscode/syntaxes/`)
- [x] Sample files exist (`examples/`)

## Temporary Solution

Until Lex is officially recognized, we use `.gitattributes`:
```
*.lex linguist-language=Text
```

This ensures `.lex` files are counted, just under "Text" category.
