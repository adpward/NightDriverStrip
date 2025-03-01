//+--------------------------------------------------------------------------
//
// File:        TempEffect.h
//
// NightDriverStrip - (c) 2018 Plummer's Software LLC.  All Rights Reserved.  
//
// This file is part of the NightDriver software project.
//
//    NightDriver is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//   
//    NightDriver is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//   
//    You should have received a copy of the GNU General Public License
//    along with Nightdriver.  It is normally found in copying.txt
//    If not, see <https://www.gnu.org/licenses/>.
//
//
// Description:
//
//    Scratchpad file for developing new effects
//
// History:     Jul-15-2021         Davepl      Created
//
//---------------------------------------------------------------------------

#pragma once

#if ENABLE_AUDIO

extern DRAM_ATTR AppTime g_AppTime;

class SimpleInsulatorBeatEffect : public LEDStripEffect, public BeatEffectBase
{
  protected:

    std::deque<int> _lit;

    virtual void Draw()
    {
        BeatEffectBase::ProcessAudio();
        fadeAllChannelsToBlackBy(min(255.0f, g_AppTime.DeltaTime() * 1500.0f));
    }

    virtual void HandleBeat(bool bMajor, float elapsed, float span)
    {
        while (_lit.size() >= NUM_FANS - 1)
            _lit.pop_front();

        size_t i;
        do
        {
            i = random(0, NUM_FANS);
        } while (_lit.end() != std::find(_lit.begin(), _lit.end(), i));
        _lit.push_back(i);
        
        FillRingPixels(RandomSaturatedColor(), i, 0);
    }

  public:

    using BeatEffectBase::BeatEffectBase;
 
    SimpleInsulatorBeatEffect(const String & strName) 
      : LEDStripEffect(EFFECT_STRIP_SIMPLE_INSULATOR_BEAT, strName), BeatEffectBase(0.5, 0.01)
    {
    }

    SimpleInsulatorBeatEffect(const JsonObjectConst& jsonObject) 
      : LEDStripEffect(jsonObject), BeatEffectBase(0.5, 0.01)
    {
    }

};

class SimpleInsulatorBeatEffect2 : public LEDStripEffect, public BeatEffectBase
{
  protected:

    std::deque<int> _lit;

    virtual void Draw()
    {
        BeatEffectBase::ProcessAudio();
        fadeAllChannelsToBlackBy(min(255.0f, g_AppTime.DeltaTime() * 1500.0f));
    }

    virtual void HandleBeat(bool bMajor, float elapsed, float span)
    {
        while (_lit.size() >= NUM_FANS - 1)
            _lit.pop_front();

        size_t i;
        do
        {
            i = random(0, NUM_FANS);
        } while (_lit.end() != std::find(_lit.begin(), _lit.end(), i));
        _lit.push_back(i);

      FillRingPixels(CRGB::Red, i, 0);        
    }

  public:
 
    SimpleInsulatorBeatEffect2(const String & strName) 
      : LEDStripEffect(EFFECT_STRIP_SIMPLE_INSULATOR_BEAT2, strName), BeatEffectBase()
    {
    }

    SimpleInsulatorBeatEffect2(const JsonObjectConst& jsonObject) 
      : LEDStripEffect(jsonObject), BeatEffectBase()
    {
    }
};

class VUInsulatorsEffect : public LEDStripEffect
{
    int _last = 1;

    using LEDStripEffect::LEDStripEffect;

    void DrawVUPixels(int i, int fadeBy, const CRGBPalette16 & palette)
    {
      CRGB c = ColorFromPalette(palette, ::map(i, 0, _cLEDs, 0, 255)).fadeToBlackBy(fadeBy);
      setPixelOnAllChannels(i, c);
    }

    virtual void Draw()
    {
      static int iPeakVUy = 0;              // Where the peak occurred
      static unsigned long msPeakVU = 0;    // Timestamp of when the last big peak was

      setAllOnAllChannels(0, 0 , 0);
      
      const int MAX_FADE = 255;

      if (iPeakVUy > 0)
      {
        int fade = MAX_FADE * ((millis() - msPeakVU) / (float) MILLIS_PER_SECOND);
        fade = min(fade, MAX_FADE);
        DrawVUPixels(iPeakVUy, fade, vu_gpGreen);
      }

      int bars = ::map(g_Analyzer._VU, g_Analyzer._MinVU, 150.0, 1, _cLEDs - 1);
      if (bars >= iPeakVUy)
      {
        msPeakVU = millis();
        iPeakVUy = bars;
      }
      else if (millis() - msPeakVU > MILLIS_PER_SECOND * 1)
      {
        iPeakVUy = 0;
      }

      const int weight = 10;
      bars = (_last * weight + bars)  / (_last * (weight + 1));
      bars = max(bars, 1);
      _last = bars;

      for (int i = 0; i < bars; i++)
        DrawVUPixels(i, 0, vuPaletteGreen);
    }  
};

#endif
