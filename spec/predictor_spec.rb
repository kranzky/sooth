require 'tempfile'

require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe Sooth::Predictor do
  let(:predictor) { Sooth::Predictor.new(42) }

  describe "#observe" do
    it "increments observation counts" do
      expect(predictor.observe([1,2], 3)).to eq(1)
      expect(predictor.observe([1,2], 3)).to eq(2)
      expect(predictor.observe([1,2], 3)).to eq(3)
    end

    it "properly sorts and finds bigrams" do
      expect(predictor.observe([2,1], 3)).to eq(1)
      expect(predictor.observe([1,3], 2)).to eq(1)
      expect(predictor.observe([3,2], 1)).to eq(1)
      expect(predictor.observe([1,3], 2)).to eq(2)
      expect(predictor.observe([2,1], 3)).to eq(2)
      expect(predictor.observe([3,2], 1)).to eq(2)
    end
  end

  describe "#count" do
    it "returns a zero count for an unobserved bigram" do
      expect(predictor.count([1,2])).to eq(0)
    end

    it "returns the total count for an observed bigram" do
      predictor.observe([1,2], 2)
      predictor.observe([1,2], 1)
      predictor.observe([1,2], 4)
      predictor.observe([1,2], 3)
      predictor.observe([1,2], 0)
      predictor.observe([1,2], 1)
      predictor.observe([1,2], 4)
      expect(predictor.count([1,2])).to eq(7)
    end
  end

  describe "#select" do
    it "returns the error symbol for an unobserved bigram" do
      expect(predictor.select([1,2], 1)).to eq(42)
    end

    it "returns the correct symbol for an observed bigram" do
      predictor.observe([1,2], 4)
      predictor.observe([1,2], 3)
      predictor.observe([1,2], 4)
      predictor.observe([1,2], 5)
      expect(predictor.select([1,2], 1)).to eq(3)
      expect(predictor.select([1,2], 2)).to eq(4)
      expect(predictor.select([1,2], 3)).to eq(4)
      expect(predictor.select([1,2], 4)).to eq(5)
    end

    it "returns the error symbol for a limit that is out of range" do
      predictor.observe([1,2], 4)
      predictor.observe([1,2], 3)
      predictor.observe([1,2], 5)
      expect(predictor.select([1,2], 0)).to eq(42)
      expect(predictor.select([1,2], 4)).to eq(42)
    end

    it "selects the correct symbol with many contexts" do
      predictor.observe([2,1], 4)
      predictor.observe([1,3], 5)
      predictor.observe([3,2], 6)
      predictor.observe([1,3], 7)
      predictor.observe([2,1], 8)
      predictor.observe([3,2], 9)
      predictor.observe([1,3], 1)
      predictor.observe([2,1], 2)
      predictor.observe([3,2], 3)
      expect(predictor.select([2,1], 0)).to eq(42)
      expect(predictor.select([2,1], 1)).to eq(2)
      expect(predictor.select([2,1], 2)).to eq(4)
      expect(predictor.select([2,1], 3)).to eq(8)
      expect(predictor.select([2,1], 4)).to eq(42)
      expect(predictor.select([1,3], 0)).to eq(42)
      expect(predictor.select([1,3], 1)).to eq(1)
      expect(predictor.select([1,3], 2)).to eq(5)
      expect(predictor.select([1,3], 3)).to eq(7)
      expect(predictor.select([1,3], 4)).to eq(42)
      expect(predictor.select([3,2], 0)).to eq(42)
      expect(predictor.select([3,2], 1)).to eq(3)
      expect(predictor.select([3,2], 2)).to eq(6)
      expect(predictor.select([3,2], 3)).to eq(9)
      expect(predictor.select([3,2], 4)).to eq(42)
    end
  end

  describe "#clear" do

    it "clears to a blank slate" do
      expect(predictor.observe([1,2], 3)).to eq(1)
      expect(predictor.observe([1,2], 3)).to eq(2)
      expect(predictor.observe([1,2], 3)).to eq(3)
      predictor.clear
      expect(predictor.observe([1,2], 3)).to eq(1)
      expect(predictor.observe([1,2], 3)).to eq(2)
      expect(predictor.observe([1,2], 3)).to eq(3)
    end

  end

  describe "#save" do

    it "can save a file and load it back again" do
      begin
        file = Tempfile.new('sooth_spec')
        expect(predictor.observe([1,2], 3)).to eq(1)
        expect(predictor.observe([2,1], 3)).to eq(1)
        expect(predictor.observe([1,2], 3)).to eq(2)
        expect(predictor.observe([1,2], 3)).to eq(3)
        expect { predictor.save(file.path) } .to_not raise_error
        expect(predictor.count([1,2])).to eq(3)
        expect(predictor.count([2,1])).to eq(1)
        predictor.clear
        expect(predictor.count([1,2])).to eq(0)
        expect(predictor.count([2,1])).to eq(0)
        expect { predictor.load(file.path) }.to_not raise_error
        expect(predictor.count([1,2])).to eq(3)
        expect(predictor.count([2,1])).to eq(1)
        expect(predictor.observe([1,2], 3)).to eq(4)
        expect(predictor.observe([1,2], 1)).to eq(1)
        expect(predictor.observe([2,1], 3)).to eq(2)
        expect(predictor.observe([2,1], 1)).to eq(1)
      ensure
        file.close
        file.unlink
      end
    end

  end

  describe "#uncertainty" do

    it "has no uncertainty for a new context" do
      expect(predictor.uncertainty([1, 2])).to be_nil 
      expect(predictor.count([1, 2])).to eq(0) 
      expect(predictor.uncertainty([1, 2])).to be_nil 
    end

    it "has zero uncertainty for a lone context" do
      predictor.observe([1, 2], 3)
      expect(predictor.uncertainty([1, 2])).to eq(0) 
    end

    it "has maximal uncertainty for a uniform distribution" do
      (1..256).each { |i| predictor.observe([1, 2], i) }
      expect(predictor.uncertainty([1, 2])).to eq(8) 
    end

  end

  describe "#surprise" do

    it "has no surprise for a new context or symbol" do
      expect(predictor.surprise([1, 2], 3)).to be_nil 
      expect(predictor.count([1, 2])).to eq(0) 
      expect(predictor.surprise([1, 2], 3)).to be_nil 
    end

    it "has zero surprise for a lone context" do
      predictor.observe([1, 2], 3)
      expect(predictor.surprise([1, 2], 3)).to eq(0) 
    end

    it "has uniform surprise for a uniform distribution" do
      (1..256).each { |i| predictor.observe([1, 2], i) }
      expect(predictor.surprise([1, 2], 3)).to eq(8) 
    end

  end

end
