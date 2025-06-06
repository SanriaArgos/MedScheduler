import {Header} from "@/app/components/Header";
import {Footer} from "@/app/components/Footer";

export default function MainDesignLayout({children}) {
    return <>
        <Header/>
        <div className="pt-18 xl:pt-30 pb-4 xl:pb-10 max-w-screen-2xl mx-auto w-full h-fit min-h-[70vh] lg:min-h-[90vh] px-2.5 2xl:px-0">
            {children}
        </div>
        <Footer/>
    </>
}
