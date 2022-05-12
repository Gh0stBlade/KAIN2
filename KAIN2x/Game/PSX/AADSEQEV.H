#ifndef AADSEQEV_H
#define AADSEQEV_H


extern int aadQueueNextEvent(struct _AadSequenceSlot *slot, int track); // 0x800548E8

extern void aadExecuteEvent(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80054ADC

extern void midiNoteOff(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80054B74

extern void midiNoteOn(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80054B7C

extern void aadUpdateChannelVolPan(struct _AadSequenceSlot *slot, int channel); // 0x80054EF0

extern void aadUpdateSlotVolPan(struct _AadSequenceSlot *slot); // 0x80055274

extern void aadUpdateChannelPitchBend(struct _AadSequenceSlot *slot, int channel); // 0x800555D0

extern void midiPolyphonicAftertouch(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055780

extern void midiControlChange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055788

extern void midiProgramChange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800557C4

extern void midiChannelAftertouch(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800557DC

extern void midiPitchWheelControl(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800557E4

extern void midiMetaEvent(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x8005582C

extern void midiControlBankSelect(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055834

extern void midiControlVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x8005583C

extern void midiControlPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x8005588C

extern void midiControlCallback(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x800558DC

extern void midiControlDummy(struct AadSeqEvent *event, struct _AadSequenceSlot *slot); // 0x80055920


#endif
