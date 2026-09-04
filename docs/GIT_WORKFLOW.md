# Git Workflow Standard

## Purpose

This document defines the standard Git workflow for developing the
project on multiple systems (for example macOS and Ubuntu).

The central rule is:

> **The remote repository (`origin`) is the shared source of truth.**

Development systems are not synchronized directly with each other.
Changes are committed and pushed from the active system to the remote
repository. Any other system is then synchronized from the remote
repository before development continues.

------------------------------------------------------------------------

## 1. Standard Workflow: Finish Work on System A

Before switching to another development system, ensure that all intended
changes are committed and pushed.

### Check the current state

``` bash
git status
git branch --show-current
```

### Commit and push changes

``` bash
git add .
git commit -m "Describe the implemented change"
git push
```

If the current branch has already been configured to track a remote
branch, `git push` is sufficient.

### First push of a new local branch

If the branch was created locally and does not yet exist on the remote:

``` bash
git push -u origin BRANCHNAME
```

The `-u` option establishes the upstream tracking relationship.
Subsequent pushes and pulls can then use:

``` bash
git push
git pull
```

At this point, the remote repository contains the current development
state.

------------------------------------------------------------------------

## 2. Continue Work on System B

Before continuing development on another system, synchronize its
knowledge of the remote repository:

``` bash
git fetch origin --prune
```

`fetch` retrieves the current remote state without modifying the working
branch. `--prune` removes obsolete remote-tracking references for
branches that have already been deleted remotely.

### Inspect available remote branches

``` bash
git branch -r
```

### Branch already exists locally

If the desired branch already exists locally:

``` bash
git switch BRANCHNAME
git pull
```

This updates the local branch from its configured upstream branch.

### Branch exists remotely but not locally

Create a local tracking branch:

``` bash
git switch --track origin/BRANCHNAME
```

For example:

``` bash
git switch --track origin/feature/D30-example
```

Git creates the corresponding local branch and configures it to track
the remote branch.

------------------------------------------------------------------------

## 3. Synchronizing an Obsolete Development System

Use this procedure only when the local state is obsolete and **no local
work needs to be preserved**.

> **Warning:** `git reset --hard` and `git branch -D` can permanently
> discard local work.

### Update remote references

``` bash
git fetch origin --prune
```

### Reset local `main` to the authoritative remote state

``` bash
git switch main
git reset --hard origin/main
```

The local `main` now points to exactly the same commit as `origin/main`.

### Remove obsolete local branches

Inspect local branches:

``` bash
git branch
```

Delete branches that are no longer required:

``` bash
git branch -D OLD-BRANCH
```

Example:

``` bash
git branch -D feature/D28-Adapt-Cockpit-Frontend-to-Full-6DoF-State
```

### Make current remote branches available locally

For a remote branch that does not yet exist locally:

``` bash
git switch --track origin/NEW-BRANCH
```

To create another tracking branch, switch away from the current branch
first:

``` bash
git switch main
git switch --track origin/ANOTHER-BRANCH
```

------------------------------------------------------------------------

## 4. Verify Branch Tracking

Use:

``` bash
git branch -vv
```

Example:

``` text
* feature/D30-foo   abc123 [origin/feature/D30-foo] Current feature
  feature/D31-bar   def456 [origin/feature/D31-bar] Another feature
  main              123abc [origin/main]            Main branch
```

The `[origin/...]` entry indicates which remote branch the local branch
tracks.

------------------------------------------------------------------------

## 5. Development-System Handover

The intended synchronization path is always:

``` text
System A
   |
   | commit + push
   v
Remote repository (origin)
   |
   | fetch + pull / switch --track
   v
System B
```

For example:

``` text
Ubuntu
  ↓ commit + push
origin
  ↓ fetch + pull
Mac
```

and later:

``` text
Mac
  ↓ commit + push
origin
  ↓ fetch + pull
Ubuntu
```

**Do not synchronize development systems directly. Always use the remote
repository as the exchange point.**

------------------------------------------------------------------------

## 6. Checklist Before Switching Computers

Before leaving the currently active development system:

``` bash
git status
git branch --show-current
git push
```

Confirm that:

-   the intended changes are committed;
-   the correct branch is checked out;
-   the branch has been pushed;
-   `git status` reports a clean working tree;
-   `git push` reports that the remote is up to date.

A typical final state is:

``` text
nothing to commit, working tree clean
Everything up-to-date
```

------------------------------------------------------------------------

## 7. Checklist When Starting on Another Computer

Always begin with:

``` bash
git fetch origin --prune
```

Then inspect the repository if necessary:

``` bash
git branch
git branch -r
```

If the desired branch already exists locally:

``` bash
git switch BRANCHNAME
git pull
```

If it exists only remotely:

``` bash
git switch --track origin/BRANCHNAME
```

------------------------------------------------------------------------

## 8. Useful Recovery Commands

### Discard uncommitted changes in the current branch

``` bash
git reset --hard
```

### Force the current local branch to match a specific remote branch

``` bash
git reset --hard origin/BRANCHNAME
```

### Remove obsolete remote-tracking references

``` bash
git fetch origin --prune
```

### Delete a local branch

``` bash
git branch -D BRANCHNAME
```

### Delete a remote branch

``` bash
git push origin --delete BRANCHNAME
```

### Show the latest commit

``` bash
git log --oneline -1
```

### Show local branches and their upstream tracking branches

``` bash
git branch -vv
```

### Show remote branches

``` bash
git branch -r
```

------------------------------------------------------------------------

## 9. Core Rules

1.  **`origin` is the shared source of truth between development
    systems.**
2.  **Commit and push before switching systems.**
3.  **Fetch before starting work on another system.**
4.  **Use `git pull` for an existing local tracking branch.**
5.  **Use `git switch --track origin/BRANCHNAME` when a remote branch
    does not yet exist locally.**
6.  **Use `git fetch origin --prune` regularly to remove obsolete remote
    references.**
7.  **Do not use `reset --hard` unless local changes and local-only
    commits may safely be discarded.**
8.  **Keep `main` synchronized with `origin/main` and perform
    development on feature branches.**
