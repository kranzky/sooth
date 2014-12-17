# Generated by jeweler
# DO NOT EDIT THIS FILE DIRECTLY
# Instead, edit Jeweler::Tasks in Rakefile, and run 'rake gemspec'
# -*- encoding: utf-8 -*-
# stub: sooth 1.0.0 ruby lib
# stub: ext/sooth_native/extconf.rb

Gem::Specification.new do |s|
  s.name = "sooth"
  s.version = "1.0.0"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib"]
  s.authors = ["Jason Hutchens"]
  s.date = "2014-12-17"
  s.description = "Sooth is a simple stochastic predictive model."
  s.email = "jasonhutchens@gmail.com"
  s.extensions = ["ext/sooth_native/extconf.rb"]
  s.extra_rdoc_files = [
    "README.md"
  ]
  s.files = [
    "Gemfile",
    "Gemfile.lock",
    "README.md",
    "Rakefile",
    "UNLICENSE",
    "VERSION",
    "ext/sooth_native/extconf.rb",
    "ext/sooth_native/native.c",
    "ext/sooth_native/sooth_context.h",
    "ext/sooth_native/sooth_predictor.c",
    "ext/sooth_native/sooth_predictor.h",
    "ext/sooth_native/sooth_statistic.h",
    "lib/sooth.rb",
    "sooth.gemspec",
    "spec/memory_spec.rb",
    "spec/predictor_spec.rb",
    "spec/spec_helper.rb"
  ]
  s.homepage = "http://github.com/jasonhutchens/sooth"
  s.licenses = ["UNLICENSE"]
  s.required_ruby_version = Gem::Requirement.new("~> 2.1")
  s.rubygems_version = "2.4.4"
  s.summary = "Sooth is a simple stochastic predictive model."

  if s.respond_to? :specification_version then
    s.specification_version = 4

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<rspec>, ["~> 3.1"])
      s.add_development_dependency(%q<yard>, ["~> 0.8"])
      s.add_development_dependency(%q<rdoc>, ["~> 4.1"])
      s.add_development_dependency(%q<bundler>, ["~> 1.7"])
      s.add_development_dependency(%q<jeweler>, ["~> 2.0"])
      s.add_development_dependency(%q<simplecov>, ["~> 0.9"])
      s.add_development_dependency(%q<byebug>, ["~> 3.5"])
      s.add_development_dependency(%q<rake-compiler>, ["~> 0.9"])
    else
      s.add_dependency(%q<rspec>, ["~> 3.1"])
      s.add_dependency(%q<yard>, ["~> 0.8"])
      s.add_dependency(%q<rdoc>, ["~> 4.1"])
      s.add_dependency(%q<bundler>, ["~> 1.7"])
      s.add_dependency(%q<jeweler>, ["~> 2.0"])
      s.add_dependency(%q<simplecov>, ["~> 0.9"])
      s.add_dependency(%q<byebug>, ["~> 3.5"])
      s.add_dependency(%q<rake-compiler>, ["~> 0.9"])
    end
  else
    s.add_dependency(%q<rspec>, ["~> 3.1"])
    s.add_dependency(%q<yard>, ["~> 0.8"])
    s.add_dependency(%q<rdoc>, ["~> 4.1"])
    s.add_dependency(%q<bundler>, ["~> 1.7"])
    s.add_dependency(%q<jeweler>, ["~> 2.0"])
    s.add_dependency(%q<simplecov>, ["~> 0.9"])
    s.add_dependency(%q<byebug>, ["~> 3.5"])
    s.add_dependency(%q<rake-compiler>, ["~> 0.9"])
  end
end

