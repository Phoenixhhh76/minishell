# minishell
yeah yeah

## ✅ **Basic Git Collaboration Workflow (Two People)**

### 🔧 1. **Start from a clean `main`**
Everyone should sync with the latest `main` before doing anything:

```bash
git checkout main
git pull origin main
```

---

### 🌿 2. **Create a feature/fix branch**
Always work in a new branch named after your task.

```bash
git checkout -b feature/ast           # for new features
# or
git checkout -b fix/lexer-bug         # for bug fixes
```

✅ **Branch naming tips:**
- Use `feature/xxx` for new features
- Use `fix/xxx` for bug fixes
- Use `refactor/xxx` for code clean-up
- No spaces or special characters (`_` or `-` are okay)

---

### ✍️ 3. **Commit your work**
Once you make changes:

```bash
git add .
git commit -m "feat: add basic AST structure"
```

✅ Good commit message format:
- `feat:` for features
- `fix:` for bug fixes
- `refactor:` for code cleanup
- `docs:` for documentation

---

### 🚀 4. **Push your branch to GitHub**

```bash
git push -u origin feature/ast
```

Then go to GitHub and **create a Pull Request (PR)**.

---

### 🔍 5. **Code review + Merge**
Your teammate reviews the PR and merges it into `main`.

---

### 🔄 6. **Update your local main before next task**

```bash
git checkout main
git pull origin main
```

---

## 📁 Suggested Directory Structure for minishell

```bash
includes/
    minishell.h
    ast.h
    lexer.h
    parser.h

src/
    ast/
    lexer/
    parser/
    builtin/
    exec/

Makefile
README.md
```

---

## 🧠 Team Tip

When you're not sure who's doing what, write it down in a shared `CONTRIBUTORS.md` or a GitHub Project/Issue:

```markdown
### Phoenix:
- Implement `ast.h` and `parser.c`

### Nina:
- Work on `lexer.c` and `tokenizer`
