# High-priority tasks

## Moving code
- [x] Moved all code in `c/normal/c_headers.*` to `c/normal/c_main.c`. Also deleted `c/normal/c_headers.*` and edited `c/normal/Makefile` to compensate.
- [x] Moved all code in `cpp/normal/cpp_args.cpp` to `cpp/normal/cpp_args.hpp`. Also deleted `cpp/normal/cpp_args.cpp` and `cpp/normal/cpp_trie.cpp` and edited `cpp/normal/Makefile` to compensate.

## Test or re-test everything
- [ ] Ensure the correctness of *each and every* function in this repo.

It might be a good idea to separate the following subtasks by file type, and handle the file types in this order:
- [x] 1. Ensure the correctness of *each and every* `Makefile` in this repo.
- [x] 2. Ensure the correctness of *each and every* function in *each and every* `.c` in this repo.
- [x] 3. Ensure the correctness of *each and every* function in *each and every* `.h` in this repo.
- [x] 4. Ensure the correctness of *each and every* function in *each and every* `.cpp` in this repo, especially the correctness of all concurrent code.
- [ ] 5. Ensure the correctness of *each and every* function in *each and every* `.hpp` in this repo, especially the correctness of all concurrent code.

## Merge approval policy AND continuous integration

<details>

<summary>2 tasks complete: Merge approval, CI</summary>

### Task list
The following two tasks are listed in the screenshot below:
- [x] Create a merge approval policy -- *[Taken care of already, it seems](#the-first-task-seems-to-have-been-taken-of-already-due-to-the-completion-of-another-task)*
- [x] Create a GitHub Action ~~(or something similar to it)~~ for continuous integration -- *I'm not sure there is anything "similar to it."*

### Screenshots

#### Both tasks
![A screenshot listing two new tasks: "Require approval from specific reviewers before merging" and "Continuous integration has not been set up"](screenshots/github/pull-request-8.png)

#### The "merge approval policy" task seems to have been taken of already due to the completion of [another task](#protecting-the-main-branch).
![Evidence that the merge approval policy may have been already been created](screenshots/github/ruleset-merge-approval.png)

#### "Suggested workflows" for this repo
![A screenshot of this repo's GitHub homepage. This screenshot lists "suggested workflows," including "C/C++ with Make."](screenshots/github/homepage-workflow-suggestions.png)

</details>

## Protecting the `main` branch

<details>

<summary>Task complete: Create a branch protection policy</summary>

### Task list
The following task is listed in the screenshot below:
- [x] Create a branch protection policy

### Screenshot
![A screenshot listing something new to fix: "Your main branch isn't protected"](screenshots/github/homepage.png)

</details>

.

.

.

.

.

# Medium-priority tasks

## Add steps to [workflow file(s)](.github/workflows/)
- [ ] Make sure that [this new YML file](.github/workflows/c-cpp.yml) has steps that run this repo's executable files after running `make` and before running `make clean`

.

.

.

.

.

# Low-priority tasks

## Add "verbose" flag (`-v`) to Makefiles
- [ ] Add the `-v` option to each use of `rm` in each Makefile in this repo

## New "deployment environments"

### Task list
The following task is listed in the screenshot below:
- [ ] Create one or more new "deployment environments," each for use with one or more rulesets

### Screenshot
![A screenshot listing something new to add: one or more "deployment environments"](screenshots/github/ruleset-deployments.png)

## New "status checks"

### Task list
The following task is listed in the screenshot below:
- [ ] Create one or more new "status checks," each for use with one or more rulesets

### Screenshot
![A screenshot listing something new to add: one or more "status checks"](screenshots/github/ruleset-status-checks.png)

.

.

.

.

.

# Special Thanks
I had some help with this TODO file from...
* https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax
* https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/about-task-lists
* https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/organizing-information-with-collapsed-sections
* https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/addressing-merge-conflicts/resolving-a-merge-conflict-on-github
* https://docs.github.com/en/get-started/learning-about-github/github-glossary#base-branch
* https://docs.github.com/en/get-started/learning-about-github/github-glossary#head-branch
