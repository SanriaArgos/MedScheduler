"use client";

import { useState } from "react";
import { useRouter } from 'next/navigation';

export const SearchBar = ({ className = "", placeholder = "Поиск врачей, клиник...", size = "small" }) => {
    const [inputState, setInputState] = useState("")
    const router = useRouter();

    const handleSearch = () => {
        const query = inputState.trim();
        if (query) {
            router.push(`/search?query=${encodeURIComponent(query)}`);
        } else {
            router.push('/search');
        }
    }

    const handleKeyPress = (e) => {
        if (e.key === 'Enter') {
            handleSearch();
        }
    }

    const sizeClasses = {
        small: "h-8 text-sm",
        medium: "h-10 text-base",
        large: "h-12 text-lg"
    };

    return (
        <div className={`flex items-center bg-white rounded-lg overflow-hidden ${className}`}>
            <input 
                type="text"
                className={`flex-1 px-3 border-none outline-none ${sizeClasses[size]}`}
                placeholder={placeholder}
                value={inputState}
                onChange={e => setInputState(e.target.value)}
                onKeyPress={handleKeyPress}
            />
            <button
                onClick={handleSearch}
                className={`px-3 bg-main2 text-white hover:bg-opacity-90 transition-colors ${sizeClasses[size]}`}
            >
                🔍
            </button>
        </div>
    );
};
