require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe Sooth::Predictor do
  let(:predictor) { Sooth::Predictor.new(42) }

  describe "#observe" do
    it "does not segfault when sorting contexts" do
      expect(predictor.observe(3, 1)).to eq(1)
      expect(predictor.observe(5, 1)).to eq(1)
      expect(predictor.observe(4, 1)).to eq(1)
      expect(predictor.observe(5, 1)).to eq(2)
      expect(predictor.observe(3, 1)).to eq(2)
      expect(predictor.observe(4, 1)).to eq(2)
      expect(predictor.observe(2, 1)).to eq(1)
      expect(predictor.observe(4, 1)).to eq(3)
      expect(predictor.observe(2, 1)).to eq(2)
      expect(predictor.observe(3, 1)).to eq(3)
      expect(predictor.observe(3, 1)).to eq(3)
    end

    it "does not segfault when sorting events" do
      expect(predictor.observe(1, 3)).to eq(1)
      expect(predictor.observe(1, 5)).to eq(1)
      expect(predictor.observe(1, 4)).to eq(1)
      expect(predictor.observe(1, 5)).to eq(2)
      expect(predictor.observe(1, 3)).to eq(2)
      expect(predictor.observe(1, 4)).to eq(2)
      expect(predictor.observe(1, 2)).to eq(1)
      expect(predictor.observe(1, 4)).to eq(3)
      expect(predictor.observe(1, 2)).to eq(2)
      expect(predictor.observe(1, 3)).to eq(3)
      expect(predictor.observe(1, 5)).to eq(3)
    end
  end
end
