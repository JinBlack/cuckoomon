/*
Cuckoo Sandbox - Automated Malware Analysis
Copyright (C) 2010-2014 Cuckoo Sandbox Developers

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <windows.h>
#include "hooking.h"
#include "ntapi.h"
#include "log.h"
#include "pipe.h"

static const char *category = "registry";
static IS_SUCCESS_LONGREG();

HOOKDEF(NTSTATUS, WINAPI, NtCreateKey,
    __out       PHANDLE KeyHandle,
    __in        ACCESS_MASK DesiredAccess,
    __in        POBJECT_ATTRIBUTES ObjectAttributes,
    __reserved  ULONG TitleIndex,
    __in_opt    PUNICODE_STRING Class,
    __in        ULONG CreateOptions,
    __out_opt   PULONG Disposition
) {
    NTSTATUS ret = Old_NtCreateKey(KeyHandle, DesiredAccess, ObjectAttributes,
        TitleIndex, Class, CreateOptions, Disposition);
    LOQ("Plopipio", "KeyHandle", KeyHandle, "DesiredAccess", DesiredAccess,
        "ObjectAttributes", unistr_from_objattr(ObjectAttributes),
        "ObjectAttributesPtr", ObjectAttributes->ObjectName->Buffer, "ObjectAttributesLen", ObjectAttributes->ObjectName->Length, 
        "ClassPtr", Class->Buffer,
        "ClassLen", Class->Length,
        "Class", Class);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtOpenKey,
    __out  PHANDLE KeyHandle,
    __in   ACCESS_MASK DesiredAccess,
    __in   POBJECT_ATTRIBUTES ObjectAttributes
) {
    NTSTATUS ret = Old_NtOpenKey(KeyHandle, DesiredAccess, ObjectAttributes);
    LOQ("Plopi", "KeyHandle", KeyHandle, "DesiredAccess", DesiredAccess,
        "ObjectAttributes", unistr_from_objattr(ObjectAttributes),
        "ObjectAttributesPtr", ObjectAttributes->ObjectName->Buffer, "ObjectAttributesLen", ObjectAttributes->ObjectName->Length);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtOpenKeyEx,
    __out  PHANDLE KeyHandle,
    __in   ACCESS_MASK DesiredAccess,
    __in   POBJECT_ATTRIBUTES ObjectAttributes,
    __in   ULONG OpenOptions
) {
    NTSTATUS ret = Old_NtOpenKeyEx(KeyHandle, DesiredAccess, ObjectAttributes,
        OpenOptions);
    LOQ("Plopi", "KeyHandle", KeyHandle, "DesiredAccess", DesiredAccess,
        "ObjectAttributes", unistr_from_objattr(ObjectAttributes),
        "ObjectAttributesPtr", ObjectAttributes->ObjectName->Buffer, "ObjectAttributesLen", ObjectAttributes->ObjectName->Length);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtRenameKey,
    __in  HANDLE KeyHandle,
    __in  PUNICODE_STRING NewName
) {
    NTSTATUS ret = Old_NtRenameKey(KeyHandle, NewName);
    LOQ("popi", "KeyHandle", KeyHandle, "NewName", NewName, 
        "NewNamePtr", NewName->Buffer, "NewNameLen", NewName->Length);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtReplaceKey,
    __in  POBJECT_ATTRIBUTES NewHiveFileName,
    __in  HANDLE KeyHandle,
    __in  POBJECT_ATTRIBUTES BackupHiveFileName
) {
    NTSTATUS ret = Old_NtReplaceKey(NewHiveFileName, KeyHandle,
        BackupHiveFileName);
    LOQ("poopp", "KeyHandle", KeyHandle,
        "NewHiveFileName", unistr_from_objattr(NewHiveFileName),
        "BackupHiveFileName", unistr_from_objattr(BackupHiveFileName),
        "NewHiveFileNamePtr", NewHiveFileName->ObjectName->Buffer,
        "NewHiveFileNameLen", NewHiveFileName->ObjectName->Length,
        "BackupHiveFileNamePtr", BackupHiveFileName->ObjectName->Buffer,
        "BackupHiveFileNameLen", BackupHiveFileName->ObjectName->Length);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtEnumerateKey,
    __in       HANDLE KeyHandle,
    __in       ULONG Index,
    __in       KEY_INFORMATION_CLASS KeyInformationClass,
    __out_opt  PVOID KeyInformation,
    __in       ULONG Length,
    __out      PULONG ResultLength
) {
    NTSTATUS ret = Old_NtEnumerateKey(KeyHandle, Index, KeyInformationClass,
        KeyInformation, Length, ResultLength);
    LOQ("pllpi", "KeyHandle", KeyHandle, "Index", Index,
        "KeyInformationClass", KeyInformationClass, "KeyInformationPtr", KeyInformation,
        "KeyInformationLen", *ResultLength);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtEnumerateValueKey,
    __in       HANDLE KeyHandle,
    __in       ULONG Index,
    __in       KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    __out_opt  PVOID KeyValueInformation,
    __in       ULONG Length,
    __out      PULONG ResultLength
) {
    NTSTATUS ret = Old_NtEnumerateValueKey(KeyHandle, Index,
        KeyValueInformationClass, KeyValueInformation, Length, ResultLength);
    LOQ("pllpi", "KeyHandle", KeyHandle, "Index", Index,
        "KeyValueInformationClass", KeyValueInformationClass, "KeyValueInformationPtr", KeyValueInformation,
        "KeyValueInformationLen", *ResultLength);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtSetValueKey,
    __in      HANDLE KeyHandle,
    __in      PUNICODE_STRING ValueName,
    __in_opt  ULONG TitleIndex,
    __in      ULONG Type,
    __in_opt  PVOID Data,
    __in      ULONG DataSize
) {
    NTSTATUS ret = Old_NtSetValueKey(KeyHandle, ValueName, TitleIndex,
        Type, Data, DataSize);
    if(NT_SUCCESS(ret)) {
        LOQ("polRpipi", "KeyHandle", KeyHandle, "ValueName", ValueName,
            "Type", Type, "Buffer", Type, DataSize, Data,
            "ValueNameBuffer", ValueName->Buffer, "ValueNameLen", ValueName->Length,
            "DataBufferPtr", Data, "DataBufferLen", DataSize);
    }
    else {
        LOQ("polpi", "KeyHandle", KeyHandle, "ValueName", ValueName,
            "Type", Type, "ValueNameBuffer", ValueName->Buffer, 
            "ValueNameLen", ValueName->Length);
    }
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtQueryValueKey,
    __in       HANDLE KeyHandle,
    __in       PUNICODE_STRING ValueName,
    __in       KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    __out_opt  PVOID KeyValueInformation,
    __in       ULONG Length,
    __out      PULONG ResultLength
) {
    ENSURE_ULONG_ZERO(ResultLength);

    NTSTATUS ret = Old_NtQueryValueKey(KeyHandle, ValueName,
        KeyValueInformationClass, KeyValueInformation, Length, ResultLength);
    if(NT_SUCCESS(ret) &&
            *ResultLength >= sizeof(ULONG) * 3) {
        ULONG Type, DataLength = 0; UCHAR *Data = NULL;

        // someday add support for Name and NameLength, if there's use for it

        Type = ((KEY_VALUE_PARTIAL_INFORMATION *) KeyValueInformation)->Type;
        if(KeyValueInformationClass == KeyValueFullInformation) {
            KEY_VALUE_FULL_INFORMATION *p =
                (KEY_VALUE_FULL_INFORMATION *) KeyValueInformation;
            DataLength = p->DataLength;
            Data = (UCHAR *) KeyValueInformation + p->DataOffset;
        }
        else if(KeyValueInformationClass == KeyValuePartialInformation) {
            KEY_VALUE_PARTIAL_INFORMATION *p =
                (KEY_VALUE_PARTIAL_INFORMATION *) KeyValueInformation;
            DataLength = p->DataLength;
            Data = p->Data;
        }

        LOQ("polRlpipi", "KeyHandle", KeyHandle, "ValueName", ValueName,
            "Type", Type, "Information", Type, DataLength, Data,
            "KeyValueInformationClass", KeyValueInformationClass, "KeyValueInformationPtr", KeyValueInformation,
            "KeyValueInformationLen", *ResultLength,
            "ValueNameBuffer", ValueName->Buffer, "ValueNameLen", ValueName->Length);
    }
    else {
        LOQ("popi", "KeyHandle", KeyHandle, "ValueName", ValueName,
            "ValueNameBuffer", ValueName->Buffer, "ValueNameLen", ValueName->Length);
    }

    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtQueryMultipleValueKey,     
    __in       HANDLE KeyHandle,
    __inout    PKEY_VALUE_ENTRY ValueEntries,     
    __in       ULONG EntryCount,
    __out      PVOID ValueBuffer,
    __inout    PULONG BufferLength,
    __out_opt  PULONG RequiredBufferLength 
) {
    NTSTATUS ret = Old_NtQueryMultipleValueKey(KeyHandle, ValueEntries, 
        EntryCount, ValueBuffer, BufferLength, RequiredBufferLength);
    LOQ("poSpipi", "KeyHandle", KeyHandle, "ValueName", ValueEntries->ValueName,
        "ValueBuffer", *BufferLength, ValueBuffer, "ValueEntriesPtr", ValueEntries,
        "ValueEntriesElements", EntryCount, "ValueBufferPtr", ValueBuffer,
        "ValueBufferLen", *BufferLength);     
    return ret; 
}

HOOKDEF(NTSTATUS, WINAPI, NtDeleteKey,
    __in  HANDLE KeyHandle
) {
    NTSTATUS ret = Old_NtDeleteKey(KeyHandle);
    LOQ("p", "KeyHandle", KeyHandle);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtDeleteValueKey,
    __in  HANDLE KeyHandle,
    __in  PUNICODE_STRING ValueName
) {
    NTSTATUS ret = Old_NtDeleteValueKey(KeyHandle, ValueName);
    LOQ("popi", "KeyHandle", KeyHandle, "ValueName", ValueName,
        "ValueNameBuffer", ValueName->Buffer, "ValueNameLen", ValueName->Length);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtLoadKey,
    __in  POBJECT_ATTRIBUTES TargetKey,
    __in  POBJECT_ATTRIBUTES SourceFile
) {
    NTSTATUS ret = Old_NtLoadKey(TargetKey, SourceFile);
    LOQ("oopipi", "TargetKey", unistr_from_objattr(TargetKey),
        "SourceFile", unistr_from_objattr(SourceFile),
        "TargetKeyPtr", TargetKey->ObjectName->Buffer, "TargetKeyLen", TargetKey->ObjectName->Length,
        "SourceFilePtr", SourceFile->ObjectName->Buffer, "SourceFileLen", SourceFile->ObjectName->Length);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtLoadKey2,
    __in  POBJECT_ATTRIBUTES TargetKey,
    __in  POBJECT_ATTRIBUTES SourceFile,
    __in  ULONG Flags
) {
    NTSTATUS ret = Old_NtLoadKey2(TargetKey, SourceFile, Flags);
    LOQ("oolpipi", "TargetKey", unistr_from_objattr(TargetKey),
        "SourceFile", unistr_from_objattr(SourceFile), "Flags", Flags,
        "TargetKeyPtr", TargetKey->ObjectName->Buffer, "TargetKeyLen", TargetKey->ObjectName->Length,
        "SourceFilePtr", SourceFile->ObjectName->Buffer, "SourceFileLen", SourceFile->ObjectName->Length);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtLoadKeyEx,
    __in      POBJECT_ATTRIBUTES TargetKey,
    __in      POBJECT_ATTRIBUTES SourceFile,
    __in      ULONG Flags,
    __in_opt  HANDLE TrustClassKey
) {
    NTSTATUS ret = Old_NtLoadKeyEx(TargetKey, SourceFile, Flags,
        TrustClassKey);
    LOQ("poolpipi", "TrustClassKey", TrustClassKey,
        "TargetKey", unistr_from_objattr(TargetKey),
        "SourceFile", unistr_from_objattr(SourceFile), "Flags", Flags,
        "TargetKeyPtr", TargetKey->ObjectName->Buffer, "TargetKeyLen", TargetKey->ObjectName->Length,
        "SourceFilePtr", SourceFile->ObjectName->Buffer, "SourceFileLen", SourceFile->ObjectName->Length);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtQueryKey,
    __in       HANDLE KeyHandle,
    __in       KEY_INFORMATION_CLASS KeyInformationClass,
    __out_opt  PVOID KeyInformation,
    __in       ULONG Length,
    __out      PULONG ResultLength
) {
    NTSTATUS ret = Old_NtQueryKey(KeyHandle, KeyInformationClass,
        KeyInformation, Length, ResultLength);
    LOQ("pSlpi", "KeyHandle", KeyHandle,
        "KeyInformation", Length, KeyInformation,
        "KeyInformationClass", KeyInformationClass, "KeyInformationPtr", KeyInformation,
        "KeyInformationLen", *ResultLength);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtSaveKey,
    __in  HANDLE KeyHandle,
    __in  HANDLE FileHandle
) {
    NTSTATUS ret = Old_NtSaveKey(KeyHandle, FileHandle);
    LOQ("pp", "KeyHandle", KeyHandle, "FileHandle", FileHandle);
    return ret;
}

HOOKDEF(NTSTATUS, WINAPI, NtSaveKeyEx,
    __in  HANDLE KeyHandle,
    __in  HANDLE FileHandle,
    __in  ULONG Format
) {
    NTSTATUS ret = Old_NtSaveKeyEx(KeyHandle, FileHandle, Format);
    LOQ("ppl", "KeyHandle", KeyHandle, "FileHandle", FileHandle,
        "Format", Format);
    return ret;
}
