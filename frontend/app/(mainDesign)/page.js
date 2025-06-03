"use client";
import Image from "next/image";
import {MainSearch} from "@/app/components/MainSearch";
import {AppPromoter} from "@/app/components/AppPromoter";

export default function Home() {
    return <>
        TEST MED
        <Image src="/icon-512.png" alt="icon" width="100" height="100"/>
        <MainSearch/>
        <AppPromoter/>
    </>
}
