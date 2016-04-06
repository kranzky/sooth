require 'tempfile'

require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe Sooth::Predictor do
  let(:error_event) { 42 }
  let(:predictor) { Sooth::Predictor.new(error_event) }

  describe "#observe" do
    it "increments counts" do
      expect(predictor.observe(1, 3)).to eq(1)
      expect(predictor.observe(1, 3)).to eq(2)
      expect(predictor.observe(1, 3)).to eq(3)
    end

    it "sorts and finds contexts" do
      expect(predictor.observe(2, 3)).to eq(1)
      expect(predictor.observe(1, 2)).to eq(1)
      expect(predictor.observe(3, 1)).to eq(1)
      expect(predictor.observe(1, 2)).to eq(2)
      expect(predictor.observe(2, 3)).to eq(2)
      expect(predictor.observe(3, 1)).to eq(2)
    end
  end

  describe "#count" do
    it "returns zero for an unobserved context" do
      expect(predictor.count(1)).to eq(0)
    end

    it "returns the number of observations" do
      predictor.observe(1, 2)
      predictor.observe(1, 1)
      predictor.observe(1, 4)
      predictor.observe(1, 3)
      predictor.observe(1, 0)
      predictor.observe(1, 1)
      predictor.observe(1, 4)
      expect(predictor.count(1)).to eq(7)
    end
  end

  describe "#size" do
    it "returns zero for an unobserved context" do
      expect(predictor.count(1)).to eq(0)
      expect(predictor.count).to eq(0)
    end

    it "returns the number of distinct contexts" do
      predictor.observe(1, 2)
      predictor.observe(2, 1)
      predictor.observe(1, 4)
      predictor.observe(1, 3)
      predictor.observe(1, 0)
      predictor.observe(2, 1)
      predictor.observe(1, 4)
      expect(predictor.count).to eq(2)
    end

    it "returns the number of distinct events when a context is given" do
      predictor.observe(1, 2)
      predictor.observe(1, 1)
      predictor.observe(1, 4)
      predictor.observe(1, 3)
      predictor.observe(1, 0)
      predictor.observe(1, 1)
      predictor.observe(1, 4)
      expect(predictor.count(1)).to eq(5)
    end

  end

  describe "#select" do
    it "returns the error event for an unobserved context" do
      expect(predictor.select(1, 1)).to eq(error_event)
    end

    it "returns the correct event for an observed context" do
      predictor.observe(1, 4)
      predictor.observe(1, 3)
      predictor.observe(1, 4)
      predictor.observe(1, 5)
      expect(predictor.select(1, 1)).to eq(3)
      expect(predictor.select(1, 2)).to eq(4)
      expect(predictor.select(1, 3)).to eq(4)
      expect(predictor.select(1, 4)).to eq(5)
    end

    it "returns the error event for a limit that is out of range" do
      predictor.observe(1, 4)
      predictor.observe(1, 3)
      predictor.observe(1, 5)
      expect(predictor.select(1, 0)).to eq(error_event)
      expect(predictor.select(1, 4)).to eq(error_event)
    end

    it "selects the correct event with many contexts" do
      predictor.observe(2, 4)
      predictor.observe(1, 5)
      predictor.observe(3, 6)
      predictor.observe(1, 7)
      predictor.observe(2, 8)
      predictor.observe(3, 9)
      predictor.observe(1, 1)
      predictor.observe(2, 2)
      predictor.observe(3, 3)
      expect(predictor.select(2, 0)).to eq(error_event)
      expect(predictor.select(2, 1)).to eq(2)
      expect(predictor.select(2, 2)).to eq(4)
      expect(predictor.select(2, 3)).to eq(8)
      expect(predictor.select(2, 4)).to eq(error_event)
      expect(predictor.select(1, 0)).to eq(error_event)
      expect(predictor.select(1, 1)).to eq(1)
      expect(predictor.select(1, 2)).to eq(5)
      expect(predictor.select(1, 3)).to eq(7)
      expect(predictor.select(1, 4)).to eq(error_event)
      expect(predictor.select(3, 0)).to eq(error_event)
      expect(predictor.select(3, 1)).to eq(3)
      expect(predictor.select(3, 2)).to eq(6)
      expect(predictor.select(3, 3)).to eq(9)
      expect(predictor.select(3, 4)).to eq(error_event)
    end
  end

  describe "#clear" do
    it "resets to a blank slate" do
      expect(predictor.observe(1, 3)).to eq(1)
      expect(predictor.observe(1, 3)).to eq(2)
      expect(predictor.observe(1, 3)).to eq(3)
      predictor.clear
      expect(predictor.observe(1, 3)).to eq(1)
      expect(predictor.observe(1, 3)).to eq(2)
      expect(predictor.observe(1, 3)).to eq(3)
    end
  end

  describe "#save" do
    it "can save a file and load it back again" do
      begin
        file = Tempfile.new('sooth_spec')
        expect(predictor.observe(1, 3)).to eq(1)
        expect(predictor.observe(2, 3)).to eq(1)
        expect(predictor.observe(1, 3)).to eq(2)
        expect(predictor.observe(1, 3)).to eq(3)
        expect { predictor.save(file.path) } .to_not raise_error
        expect(predictor.count(1)).to eq(3)
        expect(predictor.count(2)).to eq(1)
        predictor.clear
        expect(predictor.count(1)).to eq(0)
        expect(predictor.count(2)).to eq(0)
        expect { predictor.load(file.path) }.to_not raise_error
        expect(predictor.count(1)).to eq(3)
        expect(predictor.count(2)).to eq(1)
        expect(predictor.observe(1, 3)).to eq(4)
        expect(predictor.observe(1, 1)).to eq(1)
        expect(predictor.observe(2, 3)).to eq(2)
        expect(predictor.observe(2, 1)).to eq(1)
      ensure
        file.close
        file.unlink
      end
    end
  end

  describe "#distribution" do
    it "can iterate over a distribution" do
    end
  end

  describe "#uncertainty" do
    it "has no uncertainty for a new context" do
      expect(predictor.uncertainty(1)).to be_nil 
      expect(predictor.count(1)).to eq(0) 
      expect(predictor.uncertainty(1)).to be_nil 
    end

    it "has zero uncertainty for a lone context" do
      predictor.observe(1, 3)
      expect(predictor.uncertainty(1)).to eq(0) 
    end

    it "has maximal uncertainty for a uniform distribution" do
      (1..256).each { |i| predictor.observe(1, i) }
      expect(predictor.uncertainty(1)).to eq(8) 
    end
  end

  describe "#surprise" do
    it "has no surprise for a new context" do
      expect(predictor.surprise(1, 3)).to be_nil 
    end

    it "has no surprise for a new event" do
      predictor.observe(1, 3)
      expect(predictor.surprise(1, 4)).to be_nil 
    end

    it "has zero surprise for a lone event" do
      predictor.observe(1, 3)
      expect(predictor.surprise(1, 3)).to eq(0) 
    end

    it "has uniform surprise for a uniform distribution" do
      (1..256).each { |i| predictor.observe(1, i) }
      expect(predictor.surprise(1, 3)).to eq(8) 
    end
  end
end
