#! /usr/bin/fish

if begin bazel test --test_output=errors ...; end
    git commit -am working
else
    git reset HEAD --hard
end