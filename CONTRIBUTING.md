<!-- omit in toc -->
# Contributing to argparse

First off, thanks for taking the time to contribute! ❤️

All types of contributions are encouraged and valued. See the [Table of Contents](#table-of-contents) for different ways to help and details about how this project handles them. Please make sure to read the relevant section before making your contribution. It will make it a lot easier for us maintainers and smooth out the experience for all involved. The community looks forward to your contributions. 🎉

> And if you like the project, but just don't have time to contribute, that's fine. There are other easy ways to support the project and show your appreciation, which we would also be very happy about:
> - Star the project
> - Tweet about it
> - Refer this project in your project's readme
> - Mention the project at local meetups and tell your friends/colleagues

<!-- omit in toc -->
## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [I Have a Question](#i-have-a-question)
- [I Want To Contribute](#i-want-to-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements](#suggesting-enhancements)
  - [Your First Code Contribution](#your-first-code-contribution)
    - [Development Environment](#development-environment)
  - [Improving The Documentation](#improving-the-documentation)
  - [Doxygen Comment format](#doxygen-comment-format)
    - [Global constant documentation](#global-constant-documentation)
    - [Class documentation](#class-documentation)
    - [Class data member documentation](#class-data-member-documentation)
    - [Method/Function documentation](#methodfunction-documentation)
- [Styleguides](#styleguides)
  - [Commit Messages](#commit-messages)
    - [ACTION](#action)
    - [AUDIENCE](#audience)
    - [COMMIT\_MSG](#commit_msg)
    - [TAG](#tag)
- [Join The Project Team](#join-the-project-team)


## Code of Conduct

This project and everyone participating in it is governed by the
[argparse Code of Conduct](https://github.com/randaleike/argparseblob/master/CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code. Please report unacceptable behavior
to <grocsoftware@gmail.com>.


## I Have a Question

> If you want to ask a question, we assume that you have read the available [Documentation](https://github.com/randaleike/argparse/doc).

Before you ask a question, it is best to search for existing [Issues](https://github.com/randaleike/argparse/issues) that might help you. In case you have found a suitable issue and still need clarification, you can write your question in this issue. It is also advisable to search the internet for answers first.

If you then still feel the need to ask a question and need clarification, we recommend the following:

- Open an [Issue](https://github.com/randaleike/argparse/issues/new).
- Provide as much context as you can about what you're running into.
- Provide project and platform versions (nodejs, npm, etc), depending on what seems relevant.

We will then take care of the issue as soon as possible.

<!--
You might want to create a separate issue tag for questions and include it in this description. People should then tag their issues accordingly.

Depending on how large the project is, you may want to outsource the questioning, e.g. to Stack Overflow or Gitter. You may add additional contact and information possibilities:
- IRC
- Slack
- Gitter
- Stack Overflow tag
- Blog
- FAQ
- Roadmap
- E-Mail List
- Forum
-->

## I Want To Contribute

> ### Legal Notice <!-- omit in toc -->
> When contributing to this project, you must agree that you have authored 100% of the content, that you have the necessary rights to the content and that the content you contribute may be provided under the project license.

### Reporting Bugs

<!-- omit in toc -->
#### Before Submitting a Bug Report

A good bug report shouldn't leave others needing to chase you up for more information. Therefore, we ask you to investigate carefully, collect information and describe the issue in detail in your report. Please complete the following steps in advance to help us fix any potential bug as fast as possible.

- Make sure that you are using the latest version.
- Determine if your bug is really a bug and not an error on your side e.g. using incompatible environment components/versions (Make sure that you have read the [documentation](https://github.com/randaleike/argparse/doc). If you are looking for support, you might want to check [this section](#i-have-a-question)).
- To see if other users have experienced (and potentially already solved) the same issue you are having, check if there is not already a bug report existing for your bug or error in the [bug tracker](https://github.com/randaleike/argparseissues?q=label%3Abug).
- Also make sure to search the internet (including Stack Overflow) to see if users outside of the GitHub community have discussed the issue.
- Collect information about the bug:
  - Stack trace (Traceback)
  - OS, Platform and Version (Windows, Linux, macOS, x86, ARM)
  - Version of the interpreter, compiler, SDK, runtime environment, package manager, depending on what seems relevant.
  - Possibly your input and the output
  - Can you reliably reproduce the issue? And can you also reproduce it with older versions?

<!-- omit in toc -->
#### How Do I Submit a Good Bug Report?

> You must never report security related issues, vulnerabilities or bugs including sensitive information to the issue tracker, or elsewhere in public. Instead sensitive bugs must be sent by email to <grocsoftware@gmail.com>.
<!-- You may add a PGP key to allow the messages to be sent encrypted as well. -->

We use GitHub issues to track bugs and errors. If you run into an issue with the project:

- Open an [Issue](https://github.com/randaleike/argparse/issues/new). (Since we can't be sure at this point whether it is a bug or not, we ask you not to talk about a bug yet and not to label the issue.)
- Explain the behavior you would expect and the actual behavior.
- Please provide as much context as possible and describe the *reproduction steps* that someone else can follow to recreate the issue on their own. This usually includes your code. For good bug reports you should isolate the problem and create a reduced test case.
- Provide the information you collected in the previous section.

Once it's filed:

- The project team will label the issue accordingly.
- A team member will try to reproduce the issue with your provided steps. If there are no reproduction steps or no obvious way to reproduce the issue, the team will ask you for those steps and mark the issue as `needs-repro`. Bugs with the `needs-repro` tag will not be addressed until they are reproduced.
- If the team is able to reproduce the issue, it will be marked `needs-fix`, as well as possibly other tags (such as `critical`), and the issue will be left to be [implemented by someone](#your-first-code-contribution).

<!-- You might want to create an issue template for bugs and errors that can be used as a guide and that defines the structure of the information to be included. If you do so, reference it here in the description. -->


### Suggesting Enhancements

This section guides you through submitting an enhancement suggestion for argparse, **including completely new features and minor improvements to existing functionality**. Following these guidelines will help maintainers and the community to understand your suggestion and find related suggestions.

<!-- omit in toc -->
#### Before Submitting an Enhancement

- Make sure that you are using the latest version.
- Read the [documentation](https://github.com/randaleike/argparse/doc) carefully and find out if the functionality is already covered, maybe by an individual configuration.
- Perform a [search](https://github.com/randaleike/argparse/issues) to see if the enhancement has already been suggested. If it has, add a comment to the existing issue instead of opening a new one.
- Find out whether your idea fits with the scope and aims of the project. It's up to you to make a strong case to convince the project's developers of the merits of this feature. Keep in mind that we want features that will be useful to the majority of our users and not just a small subset. If you're just targeting a minority of users, consider writing an add-on/plugin library.

<!-- omit in toc -->
#### How Do I Submit a Good Enhancement Suggestion?

Enhancement suggestions are tracked as [GitHub issues](https://github.com/randaleike/argparse/issues).

- Use a **clear and descriptive title** for the issue to identify the suggestion.
- Provide a **step-by-step description of the suggested enhancement** in as many details as possible.
- **Describe the current behavior** and **explain which behavior you expected to see instead** and why. At this point you can also tell which alternatives do not work for you.
- You may want to **include screenshots and animated GIFs** which help you demonstrate the steps or point out the part which the suggestion is related to. You can use [this tool](https://www.cockos.com/licecap/) to record GIFs on macOS and Windows, and [this tool](https://github.com/colinkeenan/silentcast) or [this tool](https://github.com/GNOME/byzanz) on Linux. <!-- this should only be included if the project has a GUI -->
- **Explain why this enhancement would be useful** to most argparse users. You may also want to point out the other projects that solved it better and which could serve as inspiration.

<!-- You might want to create an issue template for enhancement suggestions that can be used as a guide and that defines the structure of the information to be included. If you do so, reference it here in the description. -->

### Your First Code Contribution
#### Development Environment
- IDE of your choice. I like vscode with C/C++, C/C++ Extension Pack, Clangd, CMake, CMake Integration and CMake Tools extentions installed.
- Platform C++ development environment with c++ standard lib version 17 or later required
- clang Version 14.0.0 or later works best
- clang-tidy Version 14.0.0 or later works best
- cmake Version 3.10 or later required
- Google unit test libraries version 1.11.0 or later required for unit testing
- Doxygen version 1.9.1 or later and graphviz to generate improved documentation.
- Github to check out and post pull requests for changes.

### Improving The Documentation
This project uses Doxygen to document the code. Update the Doxygen comments in the include file and run cmake build to generate the new documentation
### Doxygen Comment format
#### Global constant documentation
~~~
const type ConstName = <initial value>;     ///<! Brief description of the constant purpose and use
~~~
#### Class documentation
~~~
/**
 @brief Brief description of the class purpose
 
 Optional detailed description of the class purpose and use
*/
class className {
  ...
};
~~~
#### Class data member documentation
~~~
  type memberName[ = <initial value>];     ///<! Brief description of the members purpose and use
~~~
#### Method/Function documentation
~~~
/**
 @brief Brief description of the method/function purpose
 
 Optional detailed description of the method/function purpose and use

 @param arg1 - Brief description of arg1 purpose and possible values 
 @param arg2 - Brief description of arg2 purpose and possible values
 ...

 @return Description of the return1 value and meaning
 @return Description of the return2 value and meaning
 ...
**/
returnType methodName(type arg1, type arg2) 
{
  ...
}
~~~

## Styleguides
All classes, methods, functions, constants and global variables must be documented using Doxygen comments.
This project follows the [ISO C++ Core Guildlines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
for source code and the clang-tidy tool for enforcement of the guielines.

### Commit Messages
The commit message format should follow this template <ACTION: [AUDIENCE: ]COMMIT_MSG [!TAG ...]>
NOTE: The space after the semicolon is IMPORTANT!

#### ACTION
Is what the change is about.  
ACTION is required as is to be one of the following:
 - "chg" is for refactor, small improvement, cosmetic changes...
 - "fix" is for bug fixes
 - "Issue-####" is for reported bug fixes and new feature request resolution
 - "new" is for new features, big improvement

#### AUDIENCE 
AUDIENCE is WHO is concerned by the change.
The audience field is optional but encouraged and is to be one of the following:
- "dev"  is for developers (API changes, refactors...)
- "usr"  is for final users (UI changes)
- "pkg"  is for packagers   (packaging changes)
- "test" is for testers     (test only related changes)
- "doc"  is for doc guys    (doc only changes)

#### COMMIT_MSG
COMMIT_MSG is a brief description of what was modified.

#### TAG
TAGs are additionnal information describing the scope of the changes.  
They are preceded with a <!>

TAG values are: 
- "refactor" is obviously for refactoring code only
- "minor" is for a very meaningless change (a typo, adding a comment)
- "cosmetic" is for cosmetic driven change (re-indentation, 80-col...)
- "wip" is for partial functionality but complete subfunctionality.

## Join The Project Team
<!-- TODO -->

<!-- omit in toc -->
## Attribution
This guide is based on the **contributing-gen**. [Make your own](https://github.com/bttger/contributing-gen)!
